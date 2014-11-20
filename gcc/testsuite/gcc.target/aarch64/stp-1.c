/* { dg-do compile } */
/* { dg-options "-O2" } */

void f(int a, int *b)
{
  b[0] = 0;
  b[1] = a;
}

void f1(int a, int *b)
{
  b[1] = 0;
  b[0] = a;
}

void f2(int a, int *b)
{
  b[0] = 0;
  b[1] = 0;
}

/* { dg-final { scan-assembler-times "stp\\t" 3 } } */
