/* { dg-options "-O2 -mgp64" } */
/* { dg-final { scan-assembler "lw\t" } } */
/* { dg-final { scan-assembler-not "sll" } } */

struct s
{
  long long a;
  int b;
};

int
foo (struct s *x)
{
  return x->a;
}

