/* { dg-do compile } */
/* { dg-options "-O2 -march=armv8-a+lse" } */

#include "atomic-op-release.x"

/* { dg-final { scan-assembler-times "ldaddl\tw\[0-9\]+, w\[0-9\]+, \\\[x\[0-9\]+\\\]" 1 } } */
/* no ld<sub,and,nand>, so count ldaxr and stlxr instructions instead. */
/* { dg-final { scan-assembler-times "ldxr\tw\[0-9\]+, \\\[x\[0-9\]+\\\]" 3 } } */
/* { dg-final { scan-assembler-times "stlxr\tw\[0-9\]+, w\[0-9\]+, \\\[x\[0-9\]+\\\]" 3 } } */
/* { dg-final { scan-assembler-times "ldeorl\tw\[0-9\]+, w\[0-9\]+, \\\[x\[0-9\]+\\\]" 1 } } */
/* { dg-final { scan-assembler-times "ldsetl\tw\[0-9\]+, w\[0-9\]+, \\\[x\[0-9\]+\\\]" 1 } } */
