/* { dg-do compile } */
/* { dg-options "-O2 -march=armv8-a+lse" } */

#include "atomic-op-char.x"

/* { dg-final { scan-assembler-times "ldaddb\tw\[0-9\]+, w\[0-9\]+, \\\[x\[0-9\]+\\\]" 1 } } */
/* no ld<sub,and,nand>, so count ldaxr and stlxr instructions instead. */
/* { dg-final { scan-assembler-times "ldxrb\tw\[0-9\]+, \\\[x\[0-9\]+\\\]" 3 } } */
/* { dg-final { scan-assembler-times "stxrb\tw\[0-9\]+, w\[0-9\]+, \\\[x\[0-9\]+\\\]" 3 } } */
/* { dg-final { scan-assembler-times "ldeorb\tw\[0-9\]+, w\[0-9\]+, \\\[x\[0-9\]+\\\]" 1 } } */
/* { dg-final { scan-assembler-times "ldsetb\tw\[0-9\]+, w\[0-9\]+, \\\[x\[0-9\]+\\\]" 1 } } */
