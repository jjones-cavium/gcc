/* { dg-do compile } */
/* { dg-options "-O2 -fdump-tree-optimized" } */

/* We want this optimized as
<bb 2>:
  if (k != 0) goto <L2>; else goto <L3>;

<L3>:;
  i1 = j1;

<L2>:;
  return i1;

  This requires that i1 and j1 are changed into registers after they
  no longer have their address taken.  */

int f(int k, int i1, int j1)
{
  int *f1;
  if(k)
   f1 = &i1;
  else
   f1 = &j1;
  return *f1;
}

/* { dg-final { scan-tree-dump "(i1_. = PHI <i1_\[^,\]*, j1_\[^>\]*>)|(i1_. = k_..D. == 0 . j1_..D. : i1_..D.)" "optimized" } } */
/* { dg-final { scan-tree-dump "return i1_.;" "optimized" } } */
/* { dg-final { cleanup-tree-dump "optimized" } } */
