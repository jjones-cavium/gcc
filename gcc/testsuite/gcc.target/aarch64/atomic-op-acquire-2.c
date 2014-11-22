/* { dg-do compile } */
/* { dg-options "-O2 -march=armv8-a+lse" } */

#include "atomic-op-acquire.x"

/* { dg-final { scan-assembler-times "ldadda\tw\[0-9\]+, w\[0-9\]+, \\\[x\[0-9\]+\\\]" 1 } } */
/* no ld<sub,and,nand>, so count ldaxr and stlxr instructions instead. */
/* { dg-final { scan-assembler-times "ldaxr\tw\[0-9\]+, \\\[x\[0-9\]+\\\]" 3 } } */
/* { dg-final { scan-assembler-times "stxr\tw\[0-9\]+, w\[0-9\]+, \\\[x\[0-9\]+\\\]" 3 } } */
/* { dg-final { scan-assembler-times "ldeora\tw\[0-9\]+, w\[0-9\]+, \\\[x\[0-9\]+\\\]" 1 } } */
/* { dg-final { scan-assembler-times "ldseta\tw\[0-9\]+, w\[0-9\]+, \\\[x\[0-9\]+\\\]" 1 } } */
