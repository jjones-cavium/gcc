/* { dg-skip-if "not compatiable with hard-float abi" { { arm*-*-* } && arm_hf_eabi } { "-mthumb" } { "" } } */
/* { dg-options "-O2 -march=armv5te" }  */
/* { dg-final { scan-assembler "blx" } } */

int (*indirect_func)();

int indirect_call()
{
    return indirect_func();
}
