/* { dg-do compile } */
/* { dg-options "-O2 -march=armv8-a+lse" } */

#include "atomic-comp-swap-release-acquire.x"

/* { dg-final { scan-assembler-times "casal\tw\[0-9\]+, w\[0-9\]+, \\\[x\[0-9\]+\\\]" 4 } } */
