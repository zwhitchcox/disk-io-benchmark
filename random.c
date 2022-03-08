#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
int main() {
  struct timeval seed;
  gettimeofday(&seed, NULL);
  srand(seed.tv_usec);
  int num = rand() % 100;
  printf("random: %d\n", rand()%100);
}