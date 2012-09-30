/* PR target/52006 */
/* { dg-do compile } */
/* { dg-options "-march=armv7-a -mfloat-abi=hard -O2 -fPIC" } */
/* { dg-require-effective-target arm_hard_vfp_ok } */

unsigned long a;
static int b;

void
foo (void)
{
  asm volatile ("" : "=r" (b));
}

void
bar (float f)
{
  if (f < b / 100.0)
    a = 1;
}
