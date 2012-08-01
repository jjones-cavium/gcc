/* For MIPS32r2 use EXT and MIPS64r2 uses dext when ANDing with low-order bitmasks.  */
/* { dg-do compile } */
/* { dg-options "-O isa_rev>=2" } */
/* { dg-final { scan-assembler "\text|dext\t" } } */
/* { dg-final { scan-assembler-not "\tandi?\t" } } */

NOMIPS16 unsigned
f (unsigned i)
{
  return i & 0x7ffffff;
}
