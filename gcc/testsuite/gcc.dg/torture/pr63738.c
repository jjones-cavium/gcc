/*  { dg-do compile } */

#include <setjmp.h>

struct longjmp_buffer {
  jmp_buf buf;
};

void plouf();

extern long interprete()
{
  long * sp;
  int i;
  long *args;
  int n;

  struct longjmp_buffer raise_buf;
  _setjmp (raise_buf.buf);

  plouf();
  sp -= 4;
  for (i = 0; i < n; i++)
    args[i] = sp[10-i];
  plouf();
  return 0;
}
