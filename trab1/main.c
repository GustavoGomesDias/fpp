#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int global;
void * thr_func(void* args) {
  global = 40;
  printf("Nova thread: %d\n", global);
  return NULL;
}

int main(void) {
  pthread_t tid;
  global = 20;
  printf("Thread principal %d\n", global);
  pthread_create(&tid, NULL, thr_func, NULL);
  global = 30;
  pthread_join(tid, NULL);
  printf("Thread principal: %d\n", global);
  return 0;
}