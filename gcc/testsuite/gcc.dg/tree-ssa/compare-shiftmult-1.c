/* { dg-do compile } */
/* { dg-options "-O2 -fdump-tree-forwprop1-details" } */

int f(unsigned c)
{
  unsigned a = c*4;
  return (a == 0);
}

/* { dg-final { scan-tree-dump-times "gimple_simplified to _\[0-9\] = c_\[0-9\].D. & 1073741823" 2 "forwprop1" } } */
int f1(unsigned c)
{
  unsigned a = c<<2;
  return (a != 0);
}

/* { dg-final { cleanup-tree-dump "forwprop1" } } */
