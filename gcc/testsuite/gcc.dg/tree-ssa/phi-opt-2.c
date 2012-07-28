/* { dg-do compile } */
/* { dg-options "-O1 -fdump-tree-optimized" } */

_Bool f1(_Bool a, _Bool b)
{
  if (a)
   {
     if (b)
      return 1;
     else
      return 0;
   }
  return 0;
}

/* There should be only no ifs; both of them should have
   been changed into straight line code (a & b).  */
/* { dg-final { scan-tree-dump-times "if" 0 "optimized"} } */
/* { dg-final { cleanup-tree-dump "optimized" } } */
