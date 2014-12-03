/* Copyright (C) 2007 Cavium Networks.

Gcov is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2, or (at your option)
any later version.

Gcov is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Gcov; see the file COPYING.  If not, write to
the Free Software Foundation, 51 Franklin Street, Fifth Floor,
Boston, MA 02110-1301, USA.  */

#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "tm.h"
#include "version.h"
#include <getopt.h>
#include "hashtab.h"

#include <getopt.h>
#include <limits.h>
#include <glob.h>
#include <ctype.h>

#define IN_GCOV_MERGE 1
#define GCOV_LINKAGE
typedef HOST_WIDEST_INT gcov_type;
#include "gcov-io.h"
#include "gcov-io.c"

typedef void (*gcov_factored_merge_fn) (gcov_type *, gcov_unsigned_t, int);

static void factored_merge_add (gcov_type *, unsigned, int);
static void factored_merge_single (gcov_type *, unsigned, int);
static void factored_merge_delta (gcov_type *, unsigned, int);
static void factored_merge_ior (gcov_type *, unsigned, int);

/* Counts information for a function.  */
struct fn_counts
{
  /* We hash by  */
  unsigned ident;
  unsigned ctr;

  /* Store  */
  unsigned linenochecksum;
  unsigned cfgchecksum;
  gcov_type *counts;
  gcov_unsigned_t n_counts;
};

struct gcov_merge
{
  gcov_unsigned_t stamp;
  htab_t counts_hash;
  htab_t summary_hash;
};

static const gcov_factored_merge_fn merge_fn[] = GCOV_FACTORED_MERGE_FUNCTIONS;

static const struct option options[] =
{
  { "help",   no_argument, NULL, 'h' },
  { "output", required_argument, NULL, 'o' },
  { "factors",  required_argument, NULL, 'f' },
  { 0, 0, 0, 0 }
};

static void
print_usage (char *prog)
{
  fprintf (stderr,
"Usage: %s <option(s)> <files>\n"
"    or %s <option(s)> <directories>\n"
"    or %s <option(s)>\n\n"
" Merge GCC profile outpuf files.\n\n"
" In the first variant <file(s)> are merged into a single output file.  By\n"
" default the output filename is derived from the first file name by\n"
" removing the trailing numeric characters.\n"
" In the second variant, files with the identical names from <directories>\n"
" are merged into files with the same name under the current directory.\n"
" If neither <files> nor <directories> are specified each\n"
" <module>.gcda{0..31} file in the current directory is merged into a\n"
" <module>.gcda file.\n\n"
" Possible <option(s)> are:\n"
"  -o, --output=<output>  When outputting a single file (first variant) this\n"
"                         store the merged data in this file.  When\n"
"                         generating multiple files (first and second\n"
"                         variant), store the files under this directory.\n"
"  -f, --factors=f1:f2:.. When merging, weigh the first input file\n"
"                         with integer factor f1, second with f2, etc\n"
"  -h, --help             Display help message\n", prog, prog, prog);
  exit (1);
}


static hashval_t
htab_fn_counts_hash (const void *of)
{
  const struct fn_counts *entry = (const struct fn_counts *)of;

  return entry->ident * GCOV_COUNTERS + entry->ctr;
}

static int
htab_fn_counts_eq (const void *of1, const void *of2)
{
  const struct fn_counts *entry1 = (const struct fn_counts *)of1;
  const struct fn_counts *entry2 = (const struct fn_counts *)of2;

  return entry1->ident == entry2->ident && entry1->ctr == entry2->ctr;
}

static void
htab_fn_counts_del (void *of)
{
  struct fn_counts *entry = (struct fn_counts *)of;

  free (entry->counts);
  free (entry);
}

static hashval_t
htab_summary_hash (const void *of)
{
  const struct gcov_summary *entry = (const struct gcov_summary *)of;
  return entry->checksum;
}

static int
htab_summay_eq (const void *of1, const void *of2)
{
  unsigned t_ix;
  const struct gcov_summary *entry1 = (const struct gcov_summary*)of1;
  const struct gcov_summary *entry2 = (const struct gcov_summary*)of2;

  if (entry1->checksum != entry1->checksum)
    return 0;

  for (t_ix = 0; t_ix != GCOV_COUNTERS_SUMMABLE; t_ix++)
    if (entry1->ctrs[t_ix].num != entry2->ctrs[t_ix].num)
      return 0;

  return 1;
}


/* These are copies of __gcov_merge_* functions from libgcov.c.  There
   is no easy way to share them and they are pretty obvious and
   short.  */
/* The profile merging function that just adds the counters.  It is given
   an array COUNTERS of N_COUNTERS old counters and it reads the same number
   of counters from the gcov file.  */
static void
factored_merge_add (gcov_type *counters, unsigned n_counters, int factor)
{
  for (; n_counters; counters++, n_counters--)
    *counters += factor * gcov_read_counter ();
}

/* The profile merging function that just iors the counters.  It is given
   an array COUNTERS of N_COUNTERS old counters and it reads the same number
   of counters from the gcov file. This does not take into account FACTOR as
   this is to record the alignment.  */
void
factored_merge_ior (gcov_type *counters, unsigned n_counters, int factor ATTRIBUTE_UNUSED)
{
  for (; n_counters; counters++, n_counters--)
    *counters |= gcov_read_counter ();
}

/* The profile merging function for choosing the most common value.
   It is given an array COUNTERS of N_COUNTERS old counters and it
   reads the same number of counters from the gcov file.  The counters
   are split into 3-tuples where the members of the tuple have
   meanings:
   
   -- the stored candidate on the most common value of the measured entity
   -- counter
   -- total number of evaluations of the value  */
void
factored_merge_single (gcov_type *counters, unsigned n_counters, int factor)
{
  unsigned i, n_measures;
  gcov_type value, counter, all;

  gcc_assert (!(n_counters % 3));
  n_measures = n_counters / 3;
  for (i = 0; i < n_measures; i++, counters += 3)
    {
      value = gcov_read_counter ();
      counter = factor * gcov_read_counter ();
      all = factor * gcov_read_counter ();

      if (counters[0] == value)
	counters[1] += counter;
      else if (counter > counters[1])
	{
	  counters[0] = value;
	  counters[1] = counter - counters[1];
	}
      else
	counters[1] -= counter;
      counters[2] += all;
    }
}

/* The profile merging function for choosing the most common
   difference between two consecutive evaluations of the value.  It is
   given an array COUNTERS of N_COUNTERS old counters and it reads the
   same number of counters from the gcov file.  The counters are split
   into 4-tuples where the members of the tuple have meanings:
   
   -- the last value of the measured entity
   -- the stored candidate on the most common difference
   -- counter
   -- total number of evaluations of the value  */
static void
factored_merge_delta (gcov_type *counters, unsigned n_counters, int factor)
{
  unsigned i, n_measures;
  gcov_type last, value, counter, all;

  gcc_assert (!(n_counters % 4));
  n_measures = n_counters / 4;
  for (i = 0; i < n_measures; i++, counters += 4)
    {
      last = gcov_read_counter ();
      value = gcov_read_counter ();
      counter = factor * gcov_read_counter ();
      all = factor * gcov_read_counter ();

      if (counters[1] == value)
	counters[2] += counter;
      else if (counter > counters[2])
	{
	  counters[1] = value;
	  counters[2] = counter - counters[2];
	}
      else
	counters[2] -= counter;
      counters[3] += all;
    }
}

/* Merge the contents of the file FN into the global INFO.  If FIRST
   then this is the first file we reading so create the structure
   based on this.  This is based on read_counts_file.  */
static void
merge_file (struct gcov_merge *merge, const char *filename, int factor,
	    bool first)
{
  gcov_unsigned_t tag, fn_ident = 0, cfgchecksum = 0, linenochecksum = 0;
  unsigned f_ix;

  if (!gcov_open (filename, 1))
    {
      fprintf (stderr, "Unable to open %s\n", filename);
      exit (1);
    }

  if (!gcov_magic (gcov_read_unsigned (), GCOV_DATA_MAGIC))
    {
      fprintf (stderr, "File %s is not a valid data file.\n", filename);
      exit (1);
    }

  tag = gcov_read_unsigned ();
  if (tag != GCOV_VERSION)
    {
      char v[4], e[4];

      GCOV_UNSIGNED2STRING (v, tag);
      GCOV_UNSIGNED2STRING (e, GCOV_VERSION);

      fprintf (stderr, "%s is version %4s, expected version %4s",
 	       filename, v, e);
      exit (1);
    }

  tag = gcov_read_unsigned ();
  if (!first && tag != merge->stamp)
    {
      fprintf (stderr, "Stamp mismatch in file %s\n", filename);
      fprintf
	(stderr,
	 "You can't merge profile data generated for different modules\n");
      exit (1);
    }
  merge->stamp = tag;

  /* Read the program summaries */
  f_ix = 0;
  while (true)
    {
      gcov_unsigned_t length;
      struct gcov_summary summary;
      struct gcov_summary **slot, *entry;

      tag = gcov_read_unsigned ();
      if (tag != GCOV_TAG_PROGRAM_SUMMARY)
	break;
      f_ix ++;
      
      length = gcov_read_unsigned ();
      if (length != GCOV_TAG_SUMMARY_LENGTH)
	{
	  fprintf (stderr, "Merge mismatch for summary %u for file %s\n",
		   f_ix, filename);
	  exit (1);
        }
      gcov_read_summary (&summary);
      if (gcov_is_error ())
	{
	  fprintf (stderr, "Merging error for file %s\n", filename);
	  exit (1);
	}

      slot = (struct gcov_summary **)
	htab_find_slot (merge->summary_hash, &summary, INSERT);
      entry = *slot;
      if (!entry)
	{
	  *slot = entry = XCNEW (struct gcov_summary);
	  *entry = summary;
	}
      else
	{
	  unsigned i;
	  for (i = 0; i < GCOV_COUNTERS_SUMMABLE; i++)
	    {
	      struct gcov_ctr_summary *ctrs = entry->ctrs;

	      ctrs[i].num = summary.ctrs[i].num;
	      ctrs[i].runs += factor * summary.ctrs[i].runs;
	      ctrs[i].sum_all += factor * summary.ctrs[i].sum_all;
	      if (ctrs[i].run_max < factor * summary.ctrs[i].run_max)
		ctrs[i].run_max = factor * summary.ctrs[i].run_max;
	      ctrs[i].sum_max += factor * summary.ctrs[i].sum_max;
	    }
	}
    }

  /* Read and the Functions. */
  do {
      gcov_unsigned_t length;
new_function:
      if (tag == 0)
	break;

      length = gcov_read_unsigned ();

      /*  Zero lengths functions did not exit in this program
          so continue. */
      if (!length)
	{
	  tag = gcov_read_unsigned ();
	  continue;
	}
      
      if (tag != GCOV_TAG_FUNCTION)
	{
	  fprintf (stderr, "Mismatch in file %s.\n", filename);
	  fprintf (stderr, "tag (%x) is not a gcov function.\n", tag);
	  exit (1);
	}

      if (length != GCOV_TAG_FUNCTION_LENGTH)
	{
	  fprintf (stderr, "Mismatch in file %s.\n", filename);
	  exit (1);
	}

      fn_ident = gcov_read_unsigned ();
      linenochecksum = gcov_read_unsigned ();
      cfgchecksum = gcov_read_unsigned ();
      /* Read in the counters.  */

      while ((tag = gcov_read_unsigned ()))
	{
	  unsigned n_counts;
	  struct fn_counts **slot, *entry, elt;
      
	  if (!GCOV_TAG_IS_COUNTER (tag))
	    goto new_function;

	  length = gcov_read_unsigned ();
	  n_counts = GCOV_TAG_COUNTER_NUM (length);

	  elt.ident = fn_ident;
	  elt.ctr = GCOV_COUNTER_FOR_TAG (tag);

	  slot = (struct fn_counts **)
	    htab_find_slot (merge->counts_hash, &elt, INSERT);
	  entry = *slot;
	  if (!entry)
	    {
	      *slot = entry = XCNEW (struct fn_counts);
	      entry->ident = elt.ident;
	      entry->ctr = elt.ctr;
	      entry->cfgchecksum = cfgchecksum;
	      entry->linenochecksum = linenochecksum;
	      entry->n_counts = n_counts;
	      entry->counts = XCNEWVEC (gcov_type, n_counts);
	    }
	  else if (entry->cfgchecksum != cfgchecksum)
	    {
	      fprintf (stderr, "Coverage mismatch for function %u while"
		       " reading execution counters in file %s\n",
		       fn_ident, filename);
	      fprintf (stderr, "cfgchecksum is %x instead of %x\n",
		       entry->cfgchecksum, cfgchecksum);
	      exit (1);
	    }
	   else if (entry->linenochecksum != linenochecksum)
	    {
	      fprintf (stderr, "Coverage mismatch for function %u while"
		       " reading execution counters in file %s\n",
		       fn_ident, filename);
	      fprintf (stderr, "linenochecksum is %x instead of %x\n",
		       entry->linenochecksum, linenochecksum);
              exit (1);
	    }
	  else if (entry->n_counts != n_counts)
	    {
	      fprintf (stderr, "Coverage mismatch for function %u while"
		       " reading execution counters in file %s\n",
		       fn_ident, filename);
	      fprintf (stderr, "number of counters is %d instead of %d\n",
		       entry->n_counts, n_counts);
	      exit (1);
	    }

	   gcc_assert (elt.ctr < sizeof (merge_fn) / sizeof (merge_fn[0]));
	   (merge_fn[elt.ctr]) (entry->counts, n_counts, factor);
	}

    } while (0);

  fprintf (stderr, "  %s\n", filename);
  gcov_close ();
  return;
}

/* Callback function for htab_traverse.  Find each entry of
   summary, output it.  */

static int
write_summaries (void **slot, void *arg ATTRIBUTE_UNUSED)
{
  struct gcov_summary *entry = (struct gcov_summary *) *slot;
  gcov_write_summary (GCOV_TAG_PROGRAM_SUMMARY, entry);
  return 1;
}

/* Callback function for htab_traverse_noresize.  Find each entry of
   function, output it and delete the entries.  */

static int
write_function_counts (void **slot, void *arg)
{
  struct fn_counts *entry = (struct fn_counts*) *slot;
  gcov_unsigned_t fn_ident = entry->ident;
  struct gcov_merge *merge = (struct gcov_merge*) arg;
  gcov_unsigned_t i;

  gcov_write_tag_length (GCOV_TAG_FUNCTION, GCOV_TAG_FUNCTION_LENGTH);
  gcov_write_unsigned (entry->ident);
  gcov_write_unsigned (entry->linenochecksum);
  gcov_write_unsigned (entry->cfgchecksum);

  for (i = 0; i < GCOV_COUNTERS; i++)
    {
      struct fn_counts elt, *entry2;
      void **slot2;
      gcov_unsigned_t j;

      elt.ident = fn_ident;
      elt.ctr = i;

      slot2 = htab_find_slot (merge->counts_hash, &elt, NO_INSERT);
      if (!slot2)
	continue;
      entry2 = (struct fn_counts*) *slot2;
      gcov_write_tag_length (GCOV_TAG_FOR_COUNTER (i),
			     GCOV_TAG_COUNTER_LENGTH (entry2->n_counts));
      for (j = 0; j < entry2->n_counts; j++)
	gcov_write_counter (entry2->counts[j]);
      htab_clear_slot (merge->counts_hash, slot2);
    }

  return 1;
}

/* Store that data collected so far into OUTPUT_FILENAME.  This is
   based on gcov_exit.  */

static void
write_file (struct gcov_merge *merge, char *output_filename)
{
  if (!gcov_open (output_filename, -1))
    {
      fprintf (stderr, "Unable to create file %s\n", output_filename);
      exit (1);
    }

  gcov_write_tag_length (GCOV_DATA_MAGIC, GCOV_VERSION);
  gcov_write_unsigned (merge->stamp);

  htab_traverse (merge->summary_hash, write_summaries, merge);
  htab_traverse (merge->counts_hash, write_function_counts, merge);

  gcov_write_unsigned (0);

  gcov_close ();
}

static void
gcov_merge_init (struct gcov_merge *merge)
{
  merge->counts_hash =
    htab_create (10, htab_fn_counts_hash, htab_fn_counts_eq,
		 htab_fn_counts_del);
  merge->stamp = 0;
  merge->summary_hash = 
    htab_create (10, htab_summary_hash, htab_summay_eq, NULL);
}

static void
gcov_merge_clear (struct gcov_merge *merge)
{
  htab_empty (merge->counts_hash);
  merge->stamp = 0;
  htab_empty (merge->summary_hash);
}

static bool
gcov_merge_empty (struct gcov_merge *merge)
{
  return htab_elements (merge->summary_hash) == 0;
}

int
main (int argc, char **argv)
{
  int opt, i;
  char output[PATH_MAX] = { 0 };
  int nr_factors = 0;
  int *factors = NULL;
  bool dirs, files;
  struct gcov_merge gcov_merge;

  while ((opt = getopt_long (argc, argv, "ho:r:", options, NULL)) != -1)
    {
      switch (opt)
	{
	case 'h':
	  print_usage (argv[0]);
	  break;
	case 'o':
	  strncpy (output, optarg, sizeof (output));
	  break;
	case 'f':
	  {
	    unsigned i;
	    char *p;

	    for (p = optarg, nr_factors = 1; *p; p++)
	      if (*p == ':')
		nr_factors++;
	    factors = XCNEWVEC (int, nr_factors);
	    for (i = 0, p = optarg; i < (unsigned) nr_factors; i++, p++)
	      {
		factors[i] = strtoul (p, &p, 0);
		if (factors[i] == 0 || (*p && *p != ':'))
		  print_usage (argv[0]);
	      }
	    break;
	  }
	default:
	  print_usage (argv[0]);
	}
    }
  if (nr_factors > argc - optind)
    {
      if (argc - optind)
	fprintf (stderr, "More factor entries than input files\n");
      else
	fprintf (stderr, "Can't use factors with implicit file list\n");
      print_usage (argv[0]);
    }
  if (nr_factors && nr_factors < argc - optind)
    fprintf (stderr, "Fewer factor entries than input files;"
	     " assuming 1 for missing entries\n");

  /* Were we invoked with files or with dirs?  */
  dirs = files = false;
  for (i = optind; i < argc; i++)
    {
      struct stat st;
      if (stat (argv[i], &st))
	{
	  fprintf (stderr, "Unable to find file %s\n", argv[i]);
	  print_usage (argv[0]);
	}
      if (S_ISDIR (st.st_mode))
	{
	  if (files)
	    {
	      fprintf (stderr,
		       "Unable to merge files and directory %s\n", argv[i]);
	      print_usage (argv[0]);
	    }
	  dirs = true;
	}
      else
	{
	  if (dirs)
	    {
	      fprintf (stderr,
		       "Unable to merge directories and file %s\n", argv[i]);
	      print_usage (argv[0]);
	    }
	  files = true;
	}
    }
  gcov_merge_init (&gcov_merge);

  /* Explicit input file list.  */
  if (files)
    {
      if (argc - optind < 2)
	{
	  print_usage (argv[0]);
	  fprintf (stderr, "Merge at least two directories\n");
	}

      if (!output[0])
	{
	  char *filename = argv[optind];
	  char *p = strrchr (filename, '.');

	  if (strncmp (p + 1, "gcda", 4) != 0
	      || !ISDIGIT (p[5]))
	    {
	      fprintf (stderr, "Specify output with -o\n"
		       "First input file %s does not end with .gcda{0..31}\n",
		       argv[0]);
	      print_usage (argv[0]);
	    }
	  strncpy (output, filename, p + 5 - filename);
	}

      fprintf (stderr, "Merging these files into %s:\n", output);

      for (i = optind; i < argc; i++)
	merge_file (&gcov_merge, argv[i],
		    i - optind < nr_factors ? factors[i - optind] : 1,
		    i == optind);

      write_file (&gcov_merge, output);
    }

  /* Implicit input file list.  */
  else if (!dirs)
    {
      glob_t gcda_glob;
      size_t i;
      char output_to[PATH_MAX];
      char *basename = output_to;

      if (output[0])
	basename = sprintf (output_to, "%s/", output) + output_to;
      else
	output_to[0] = '\0';

      if (glob ("*.gcda[0-9]*", GLOB_MARK, NULL, &gcda_glob)
	  || !gcda_glob.gl_pathc)
	{
	  fprintf (stderr, "Could not find any intput files\n");
	  print_usage (argv[0]);
	}

      /* The output of glob is sorted.  */
      for (i = 0; i < gcda_glob.gl_pathc; i++)
	{
	  char *filename = gcda_glob.gl_pathv[i];
	  char *p, *r;
	  int core;

	  /* Stricter check on the extension part.  */
	  if (!(p = strrchr (filename, '.'))
	      || strncmp (p, ".gcda", 5) != 0)
	    continue;
	  core = strtoul (p + 5, &r, 10);
	  if (p == r
	      || *r != '\0'
	      || core < 0 || (128+47) < core)
	    continue;

	  if (strncmp (basename, filename, p + 5 - filename) != 0)
	    {
	      if (!gcov_merge_empty (&gcov_merge))
		{
		  write_file (&gcov_merge, output_to);
		  gcov_merge_clear (&gcov_merge);
		}
	      strncpy (basename, filename, p + 5 - filename);
	      basename[p + 5 - filename] = '\0';
	      fprintf (stderr, "Merging these files into %s:\n", output_to);

	      merge_file (&gcov_merge, filename, 1, true);
	    }
	  else
	    merge_file (&gcov_merge, filename, 1, false);
	}
      write_file (&gcov_merge, output_to);

      globfree (&gcda_glob);
    }

  /* Merge dirs.  */
  else
    {
      glob_t gcda_glob;
      size_t j;
      char pattern[PATH_MAX];

      if (argc - optind < 2)
	{
	  fprintf (stderr, "Merge at least two directories\n");
	  print_usage (argv[0]);
	}

      sprintf (pattern, "%s/*.gcda", argv[optind]);

      if (glob (pattern, GLOB_MARK, NULL, &gcda_glob) || !gcda_glob.gl_pathc)
	{
	  fprintf (stderr, "Could not find any intput files\n");
	  print_usage (argv[0]);
	}

      for (j = 0; j < gcda_glob.gl_pathc; j++)
	{
	  char *filename = gcda_glob.gl_pathv[j];
	  char output_filename[PATH_MAX];
	  char *output_to;

	  /* Ignore the directory.  */
	  filename = strrchr (filename, '/') + 1;
	  if (output[0])
	    {
	      output_to = output_filename;
	      sprintf (output_to, "%s/%s", output, filename);
	    }
	  else
	    output_to = filename;

	  fprintf (stderr, "Merging these files into %s:\n", output_to);

	  for (i = optind; i < argc; i++)
	    {
	      char dir_filename[PATH_MAX];

	      sprintf (dir_filename, "%s/%s", argv[i], filename);
	      merge_file (&gcov_merge, dir_filename,
			  i - optind < nr_factors ? factors[i - optind] : 1,
			  i == optind);
	    }

	  write_file (&gcov_merge, output_to);
	  gcov_merge_clear (&gcov_merge);
	}

      globfree (&gcda_glob);
    }

  return 0;
}
