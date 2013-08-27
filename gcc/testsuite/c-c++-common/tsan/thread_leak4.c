
/* { dg-do run { target { x86_64*-*-linux* } } } */
/* { dg-shouldfail "tsan" } */

#include <pthread.h>
#include <unistd.h>
#include <stdio.h>

void *Thread(void *x) {
  sleep(10);
  return 0;
}

int main() {
  pthread_t t;
  pthread_create(&t, 0, Thread, 0);
  printf("OK\n");
  return 0;
}
/* { dg-output "WARNING: ThreadSanitizer: thread leak.*" } */
