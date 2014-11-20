/* { dg-do compile { target *-octeon-* } } */
/* { dg-options "-O2" } */
/* { dg-final { scan-assembler "beq\t.*,\\\$0," } } */

void g(void);

void f (int i)
{
  if (__builtin_expect (i < 9, 1))
    g ();
}
