/* { dg-do compile } */
/* { dg-options "-O2" }  */

/* { dg-final { scan-assembler-times "fcmpe\t" 1 } } */
/* { dg-final { scan-assembler-times "csinc\t" 1 } } */

int f(float a, float b, int t)
{
  int t1 = t + 1;
  return a>=b?t:t1;
}

