/* { dg-do compile { target *-octeon-* } } */
/* { dg-options "-O2" } */
/* { dg-final { scan-assembler-not "sll" } } */

void abort (void);

void f (unsigned char c)
{
  if (c != 0xFF)
    abort ();
}
