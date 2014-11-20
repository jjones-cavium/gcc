/* None of these extractions need shifting later.  */
/* { dg-options "isa_rev>=2" } */
/* { dg-skip-if "code quality test" { *-*-* } { "-O0" } { "" } } */
/* { dg-final { scan-assembler-not "sll" } } */
/* { dg-final { scan-assembler-times "ext\\t" 5 } } */


struct s
{
  unsigned long long a:2;
  unsigned long long b:2;
  unsigned long long c:20;
  unsigned long long d:40;
};

void z(void);

/* Both of these are subject to optimize_bit_field_compare so we
   similar things in backend as in the f* functions below.  */

void f(struct s s)
{
  if (s.b == 2)
    z ();
}

void ff(struct s s)
{
  if (s.b == 1)
    z ();
}

/* Note that the shift values on the two sides of the comparison are
   different.  */

void g (unsigned long long i)
{
  if ((i & 0xfff0000000000ull) == 0x1200000000000ull)
    z ();
}


void gg (unsigned long long i)
{
  if ((i & 0xfff0000000000ull) == 0x1230000000000ull)
    z ();
}


void fff (unsigned long long i)
{
  if (i & 0xfff0000000000ull)
    z ();
}
