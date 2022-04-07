#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
int benchmark_random() {
  struct timeval seed;
  gettimeofday(&seed, NULL);
  srand(seed.tv_usec);
  int num = rand() % 100;
  return num;
}