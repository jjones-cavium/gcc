/* { dg-do compile } */
/* { dg-options "-O2 -fdump-tree-forwprop1-details" } */

int f(char *b, __SIZE_TYPE__ c)
{
  char *a = b + c;
  return (a == b);
}

/* { dg-final { scan-tree-dump-times "gimple_simplified to _\[0-9\] = c_\[0-9\].D. == 0" 1 "forwprop1" } } */
int f1(char *b, __SIZE_TYPE__ c)
{
  char *a = b + c;
  return (a != b);
}

/* { dg-final { scan-tree-dump-times "gimple_simplified to _\[0-9\] = c_\[0-9\].D. != 0" 1 "forwprop1" } } */
/* { dg-final { scan-tree-dump-times "Applying pattern" 2 "forwprop1" } } */
/* { dg-final { cleanup-tree-dump "forwprop1" } } */
