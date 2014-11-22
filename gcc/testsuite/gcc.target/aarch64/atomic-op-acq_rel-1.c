/* { dg-do compile } */
/* { dg-options "-O2 -march=armv8-a+nolse" } */

#include "atomic-op-acq_rel.x"

/* { dg-final { scan-assembler-times "ldaxr\tw\[0-9\]+, \\\[x\[0-9\]+\\\]" 6 } } */
/* { dg-final { scan-assembler-times "stlxr\tw\[0-9\]+, w\[0-9\]+, \\\[x\[0-9\]+\\\]" 6 } } */
