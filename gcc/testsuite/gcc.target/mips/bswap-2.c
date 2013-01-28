/* { dg-do compile } */
/* { dg-options "-O -march=mips32r2" } */
/* { dg-final { scan-assembler "\twsbh\t" } } */

short g(short a)
{
  return __builtin_bswap16(a);
}

