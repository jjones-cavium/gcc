/* Test the `ldp & stp' AArch64 SIMD intrinsic.  */

/* { dg-do compile } */
/* { dg-options "-Ofast" } */

#define __vector__ __attribute__((__vector_size__(16) ))

struct f_1
{
  __vector__ int a;
  __vector__ float b;
};

void
g (struct f_1 *in, struct f_1 *out)
{
  __vector__ int a = in->a;
  __vector__ float b = in->b;
  a += (__vector__ int) {1, 2, 3, 4};
  b += (__vector__ float) {1.0f, 2.0f, 3.0f, 4.0f};
  out->a = a;
  out->b = b;
}

struct f_2
{
  __vector__ int a;
  __vector__ long b;
};

void
g_1 (struct f_2 *in, struct f_2 *out)
{
  __vector__ int a = in->a;
  __vector__ long b = in->b;
  a += (__vector__ int) {1, 2, 3, 4};
  b += (__vector__ long) {1, 3};
  out->a = a;
  out->b = b;
}

struct f_3
{
  __vector__ double a;
  __vector__ float b;
};

void
g_2 (struct f_3 *in, struct f_3 *out)
{
  __vector__ double a = in->a;
  __vector__ float b = in->b;
  a += (__vector__ double) {11.0d, 12.0d};
  b += (__vector__ float) {1.0f, 2.0f, 3.0f, 4.0f};
  out->a = a;
  out->b = b;
}

struct f_4
{
  __vector__ double a;
  __vector__ long b;
};

void
g_3 (struct f_4 *in, struct f_4 *out)
{
  __vector__ double a = in->a;
  __vector__ long b = in->b;
  a += (__vector__ double) {11.0d, 12.0d};
  b += (__vector__ long) {1, 3};
  out->a = a;
  out->b = b;
}

/* { dg-final { scan-assembler-times "ldp\tq" 4 } } */
/* { dg-final { scan-assembler-times "stp\tq" 4 } } */
