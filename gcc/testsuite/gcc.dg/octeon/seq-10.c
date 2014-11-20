/* { dg-do compile { target *-octeon-* } } */
/* { dg-options "-O2" } */
/* { dg-final { scan-assembler "bne\t.*,\\\$0," } } */

void g(void);

void f (int i)
{
  if (__builtin_expect (i < 9, 0))
    g ();
}
