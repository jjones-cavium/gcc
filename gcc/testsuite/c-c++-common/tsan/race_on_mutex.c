
/* { dg-do run { target { x86_64*-*-linux* } } } */
/* { dg-shouldfail "tsan" } */

#include <pthread.h>
#include <stdio.h>
#include <stddef.h>
#include <unistd.h>

pthread_mutex_t Mtx;
int Global;

void *Thread1(void *x) {
  pthread_mutex_init(&Mtx, 0);
  pthread_mutex_lock(&Mtx);
  Global = 42;
  pthread_mutex_unlock(&Mtx);
  return NULL;
}

void *Thread2(void *x) {
  sleep(1);
  pthread_mutex_lock(&Mtx);
  Global = 43;
  pthread_mutex_unlock(&Mtx);
  return NULL;
}

int main() {
  pthread_t t[2];
  pthread_create(&t[0], NULL, Thread1, NULL);
  pthread_create(&t[1], NULL, Thread2, NULL);
  pthread_join(t[0], NULL);
  pthread_join(t[1], NULL);
  pthread_mutex_destroy(&Mtx);
  return 0;
}

/* { dg-output "WARNING: ThreadSanitizer: data race.*" } */

// CHECK:      WARNING: ThreadSanitizer: data race
// CHECK-NEXT:   Atomic read of size 1 at {{.*}} by thread T2:
// CHECK-NEXT:     #0 pthread_mutex_lock
// CHECK-NEXT:     #1 Thread2{{.*}} {{.*}}race_on_mutex.c:20{{(:3)?}} ({{.*}})
// CHECK:        Previous write of size 1 at {{.*}} by thread T1:
// CHECK-NEXT:     #0 pthread_mutex_init {{.*}} ({{.*}})
// CHECK-NEXT:     #1 Thread1{{.*}} {{.*}}race_on_mutex.c:11{{(:3)?}} ({{.*}})
