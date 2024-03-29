/* { dg-do compile } */
/* { dg-options "-O2 -mfix-r10000 forbid_cpu=octeon2" } */
/* { dg-final { scan-assembler-times "\tbeql\t" 3 } } */

NOMIPS16 int
f1 (int *z)
{
  return __sync_fetch_and_add (z, 42);
}

NOMIPS16 short
f2 (short *z)
{
  return __sync_fetch_and_add (z, 42);
}

NOMIPS16 char
f3 (char *z)
{
  return __sync_fetch_and_add (z, 42);
}
