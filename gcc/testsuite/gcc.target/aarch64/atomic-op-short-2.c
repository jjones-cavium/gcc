/* { dg-do compile } */
/* { dg-options "-O2 -march=armv8-a+lse" } */

#include "atomic-op-short.x"

/* { dg-final { scan-assembler-times "ldaddh\tw\[0-9\]+, w\[0-9\]+, \\\[x\[0-9\]+\\\]" 1 } } */
/* no ld<sub,and,nand>, so count ldaxr and stlxr instructions instead. */
/* { dg-final { scan-assembler-times "ldxrh\tw\[0-9\]+, \\\[x\[0-9\]+\\\]" 3 } } */
/* { dg-final { scan-assembler-times "stxrh\tw\[0-9\]+, w\[0-9\]+, \\\[x\[0-9\]+\\\]" 3 } } */
/* { dg-final { scan-assembler-times "ldeorh\tw\[0-9\]+, w\[0-9\]+, \\\[x\[0-9\]+\\\]" 1 } } */
/* { dg-final { scan-assembler-times "ldseth\tw\[0-9\]+, w\[0-9\]+, \\\[x\[0-9\]+\\\]" 1 } } */
