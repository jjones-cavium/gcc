/* { dg-do assemble { target *-octeon-* } } */
/* { dg-options "-O2 -mabi=32" } */

void saa (int *p, int i)
{
  asm ("saa %1,%0" : "=m"(p[10]) : "r"(i));
}
