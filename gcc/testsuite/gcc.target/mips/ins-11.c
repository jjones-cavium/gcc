/* { dg-do compile } */
/* { dg-options "-O2 isa_rev>=2 -mgp64" } */
/* { dg-final { scan-assembler-times "\tdins\t" 1 } } */
/* { dg-final { scan-assembler-not "\tdsll\t" } } */

union f
{
  long long ll;
  int l[2];
};

#if __BYTE_ORDER__==__ORDER_LITTLE_ENDIAN__
#define offset 0
#else
#define offset 1
#endif

long long g(int a, int b)
{
  union f t;
  t.ll = ((long long)a)<<32;
  t.l[offset] = b;
  return t.ll;
}

