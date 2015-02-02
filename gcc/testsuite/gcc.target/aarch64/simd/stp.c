/* Test the `stp' AArch64 SIMD intrinsic.  */

/* { dg-do compile } */
/* { dg-options "-Ofast" } */

#include <string.h>
int g_int (void *, int *, int *);
int g_long (void *, long *, long *);
int g_float (void *, float *, float *);
int g_double (void *, double *, double *);

int
f (void)
{
  int i;

  int pawns_int[2][11], white_back_pawn_int[11], black_back_pawn_int[11];
  memset (pawns_int, 0, sizeof (pawns_int));
  for (i = 0; i < 11; i++)
    {
      white_back_pawn_int[i] = 7;
      black_back_pawn_int[i] = 2;
    }
  g_int (pawns_int, white_back_pawn_int, black_back_pawn_int);

  long pawns_long[2][11], white_back_pawn_long[11], black_back_pawn_long[11];
  memset (pawns_long, 0, sizeof (pawns_long));
  for (i = 0; i < 11; i++)
    {
      white_back_pawn_long[i] = 7;
      black_back_pawn_long[i] = 2;
    }
  g_long (pawns_long, white_back_pawn_long, black_back_pawn_long);

  float pawns_float[2][11], white_back_pawn_float[11],
    black_back_pawn_float[11];
  memset (pawns_float, 0, sizeof (pawns_float));
  for (i = 0; i < 11; i++)
    {
      white_back_pawn_float[i] = 7;
      black_back_pawn_float[i] = 2;
    }
  g_float (pawns_float, white_back_pawn_float, black_back_pawn_float);

  double pawns_double[2][11], white_back_pawn_double[11],
    black_back_pawn_double[11];
  memset (pawns_double, 0, sizeof (pawns_double));
  for (i = 0; i < 11; i++)
    {
      white_back_pawn_double[i] = 7;
      black_back_pawn_double[i] = 2;
    }
  g_double (pawns_double, white_back_pawn_double, black_back_pawn_double);
}

/* { dg-final { scan-assembler-times "stp\tq" 5 {target lp64} } } */
/* { dg-final { scan-assembler-times "stp\tq" 11 {target ilp32} } } */
