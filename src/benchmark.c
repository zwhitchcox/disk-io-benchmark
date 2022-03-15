#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <math.h>

#include "benchmark.h"

long millis(){
    struct timespec _t;
    clock_gettime(CLOCK_REALTIME, &_t);
    return _t.tv_sec*1000 + lround(_t.tv_nsec/1e6);
}

int benchmark_read(struct BenchmarkOptions *o) {
  int file = open(o->file, o->open_flags, 0666);
  if (!file) {
    printf("Error opening source file '%s': %s\n", o->file, strerror(errno));
  }
  char buf[o->page_size];
  long total_bytes_read = 0, bytes_read;
  long start = millis();
  while (total_bytes_read < o->num_bytes) {
    bytes_read = read(file, buf, o->page_size);
    if (bytes_read < 0 && errno != EAGAIN) {
      fprintf(stderr, "%s\n", strerror(errno));
      return 1;
    }
    total_bytes_read += bytes_read;
  }
  print_time(o->num_bytes, millis() - start);
}

int benchmark_write(struct BenchmarkOptions *o) {
  int file = open(o->file, o->open_flags, 0666);
  if (!file) {
    printf("Error opening source file '%s': %s\n", o->file, strerror(errno));
  }
  char buf[o->page_size];
  long total_bytes_read = 0, bytes_read;
  long start = millis();
  // random string
  char *hello = "hello";
  int len = strlen(hello);
  for (int i = 0; i < o->page_size; i++) {
    buf[i] = hello[i % len];
  }
  while (total_bytes_read < o->num_bytes) {
    bytes_read = write(file, buf, o->page_size);
    if (bytes_read < 0 && errno != EAGAIN) {
      fprintf(stderr, "%s\n", strerror(errno));
      return 1;
    }
    total_bytes_read += bytes_read;
  }
  long end = millis();
  long time = end - start;
  print_time(o->num_bytes, millis() - start);
}


int benchmark(char *file) {
  benchmark_read(file);
  benchmark_write(file);
}