/* { dg-do run } */
/* { dg-require-effective-target arm_eabi } */
/* { dg-require-effective-target arm_arch_v6_multilib } */
/* { dg-skip-if "not compatiable with hard-float abi" { { arm*-*-* } && arm_hf_eabi } { "*" } { "" } } */
/* { dg-options "-mthumb" } */
/* { dg-add-options arm_arch_v6 } */

#include "ftest-support-thumb.h"

int
main (void)
{
  return ftest (ARCH_V6);
}

