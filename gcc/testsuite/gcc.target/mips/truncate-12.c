/* The second shift can be performed on 32-bit operands.  */
/* { dg-options "-O2 -mgp64" } */
/* { dg-final { scan-assembler-not "dext" } } */
/* { dg-final { scan-assembler-not "and" } } */
/* { dg-final { scan-assembler-not "\tsll\t.*,0" } } */

typedef unsigned long long uint64;
unsigned
g (uint64 x, int y, unsigned z, uint64 * p)
{
  unsigned w = ((x >> (y & 0x3f)) & 0xffffffffULL) << (z & 0x1f);
  return w;
}
