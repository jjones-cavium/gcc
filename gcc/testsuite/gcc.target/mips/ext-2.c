/* { dg-do compile } */
/* { dg-options "-O isa_rev>=2 -mgp64" } */
/* { dg-final { scan-assembler-not "\tdext\t" } } */
/* { dg-final { scan-assembler-not "and" } } */
/* { dg-final { scan-assembler "srl" } } */

NOMIPS16 void
f (unsigned char x, unsigned char *r)
{
  *r = 0x50 | (x >> 4);
}
