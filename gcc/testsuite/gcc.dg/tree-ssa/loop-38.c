/* { dg-do compile } */
/* { dg-options "-O1 -fdump-tree-ch" } */

typedef struct basket
{
    int *a;
    int cost;
    int abs_cost;
} BASKET;
BASKET *perm[50 +300 +1];


int f(int a, int *b, int cut)
{
  do {
  while (perm[a]->abs_cost > cut)
    a++;
  a++;
  } while (b[a]);
  return a;
}

/* { dg-final { scan-tree-dump-times "Disambiguating loop" 1 "ch" } } */
/* { dg-final { scan-tree-dump-times "3 loops found" 1 "ch" } } */

/* { dg-final { cleanup-tree-dump "ch" } } */

