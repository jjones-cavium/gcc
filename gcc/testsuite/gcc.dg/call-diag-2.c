/* Test diagnostics for calling function returning qualified void or
   other incomplete type other than void.  PR 35210.  */
/* { dg-do compile } */
/* { dg-options "-pedantic-errors" } */

const void f_cv (void);
struct s f_s (void);
void f_v (void);

void g1 (void) { f_cv (); } /* { dg-error "qualified void" } */
void g2 (void) { f_s (); } /* { dg-error "invalid use of undefined type" } */
void g3 (void) { ((const void (*) (void)) f_v) (); } /* { dg-error "qualified void" } */
/* { dg-error "called through a non-compatible type" "cast" { target *-*-* } 12 } */

void g4 (void) { ((struct s (*) (void)) f_v) (), (void) 0; } /* { dg-error "invalid use of undefined type" } */
/* { dg-error "called through a non-compatible type" "cast" { target *-*-* } 15 } */
