/* { dg-do compile { target *-octeon-* } } */
/* { dg-options "-O2" } */
/* { dg-final { scan-assembler "ins\t" } } */
/* { dg-final { scan-assembler-not "\texts\t" } } */
/* { dg-final { scan-assembler-not "\tsll\t" } } */
/* { dg-final { scan-assembler-not "\tdsll\t" } } */
/* { dg-final { scan-assembler-not "\tandi\t" } } */
/* { dg-final { scan-assembler-not "\tor\t" } } */

struct s 
{ 
  char c; 
  unsigned short s; 
} __attribute__ ((packed));

int f (struct s *s) 
{ 
  return s->s;
} 

