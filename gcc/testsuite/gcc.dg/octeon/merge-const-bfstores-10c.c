/* { dg-do compile { target *-octeon-* } } */
/* { dg-options "-O2" } */
/* { dg-final { scan-assembler {0x89} } } */

struct s
{
  long long d:52;
  long long a:4;
  long long b:4;
  long long c:4;
};

void h(int, struct s);
void g(int, struct s);
void f (int i)
{
  struct s s;
  s.a = 8;
  s.b = 9;
  s.c = i;
  if (i)
    h (i, s);
  else
    g (i, s);
}
