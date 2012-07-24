/* { dg-do compile { target *-octeon-* } } */
/* { dg-options "-O2" } */
/* { dg-final { scan-assembler "dins\t" } } */
/* { dg-final { scan-assembler-not "dsra\t" } } */
/* { dg-final { scan-assembler-not "dsll\t" } } */


typedef unsigned long long u64;
void f(u64 *aa, u64 a)
{
  u64 b = a>>32;
  int c = b;
  u64 d = c;
  *aa = d<<32;
}
