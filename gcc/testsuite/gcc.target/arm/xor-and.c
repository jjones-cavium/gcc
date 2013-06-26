/* { dg-do compile } */
/* { dg-skip-if "not compatiable with hard-float abi" { { arm*-*-* } && arm_hf_eabi } { "-mthumb" } { "" } } */
/* { dg-options "-O -march=armv6" } */
/* { dg-prune-output "switch .* conflicts with" } */

unsigned short foo (unsigned short x)
{
  x ^= 0x4002;
  x >>= 1;
  x |= 0x8000;
  return x;
}

/* { dg-final { scan-assembler "orr" } } */
/* { dg-final { scan-assembler-not "mvn" } } */
/* { dg-final { scan-assembler-not "uxth" } } */
