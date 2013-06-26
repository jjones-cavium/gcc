/* { dg-do compile } */
/* { dg-skip-if "not compatiable with hard-float abi" { { arm*-*-* } && arm_hf_eabi } { "-mthumb" } { "" } } */
/* { dg-options "-O2 -march=armv6" } */

unsigned char foo (unsigned char c)
{
  return (c >= '0') && (c <= '9');
}

/* { dg-final { scan-assembler-not "uxtb" } } */
