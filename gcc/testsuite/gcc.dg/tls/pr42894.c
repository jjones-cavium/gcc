/* PR target/42894 */
/* { dg-do compile } */
/* { dg-options "-march=armv5te -mthumb" { target { arm*-*-* && { ! arm_hf_eabi } } } } */
/* { dg-require-effective-target tls } */

extern __thread int t;

void
foo (int a)
{
  t = a;
}
