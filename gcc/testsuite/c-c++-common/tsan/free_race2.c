
/* { dg-do run { target { x86_64*-*-linux* } } } */
/* { dg-shouldfail "tsan" } */

#include <stdlib.h>

void __attribute__((noinline)) foo(int *mem) {
  free(mem);
}

void __attribute__((noinline)) bar(int *mem) {
  mem[0] = 42;
}

int main() {
  int *mem = (int*)malloc(100);
  foo(mem);
  bar(mem);
  return 0;
}

/* { dg-output "WARNING: ThreadSanitizer: heap-use-after-free.*" } */

// CHECK:   Write of size 4 at {{.*}} by main thread:
// CHECK:     #0 bar
// CHECK:     #1 main
// CHECK:   Previous write of size 8 at {{.*}} by main thread:
// CHECK:     #0 free
// CHECK:     #{{1|2}} foo
// CHECK:     #{{2|3}} main
