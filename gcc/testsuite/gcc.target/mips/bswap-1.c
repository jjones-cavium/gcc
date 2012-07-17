/* { dg-do compile } */
/* { dg-options "-O -mgp64 -march=mips64r2" } */
/* { dg-final { scan-assembler "\tdsbh\t" } } */
/* { dg-final { scan-assembler "\tdshd\t" } } */
/* { dg-final { scan-assembler "\twsbh\t" } } */
/* { dg-final { scan-assembler "\trotr\t" } } */

long long f(long long a)
{
  return __builtin_bswap64(a);
}
int g(int a)
{
  return __builtin_bswap32(a);
}

