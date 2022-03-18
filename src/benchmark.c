#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <math.h>

#include "common.h"
#include "benchmark.h"
#include "results.h"

ull millis(){
    struct timespec _t;
    clock_gettime(CLOCK_REALTIME, &_t);
    return _t.tv_sec*1000 + lround(_t.tv_nsec/1e6);
}

struct BenchmarkResults *benchmark_read(struct BenchmarkOpts *o) {
  int file;
  if ((file = open(o->io, O_RDONLY, 0666)) <= 0) {
    printf("Error opening source file '%s': %s\n", o->io, strerror(errno));
    exit(1);
  }
  char buf[o->page_size];
  ull total_bytes_read = 0, bytes_read;
  ull start = millis();
  while (total_bytes_read < o->bytes) {
    bytes_read = read(file, buf, o->page_size);
    if (bytes_read < 0 && errno != EAGAIN) {
      fprintf(stderr, "Read error: %s\n", strerror(errno));
      exit(1);
    }
    total_bytes_read += bytes_read;
  }
  struct BenchmarkResults *results = malloc(sizeof(struct BenchmarkResults));
  results->time = millis() - start;
  results->bytes = total_bytes_read;
  results->opts = o;
  return results;
}

struct BenchmarkResults *benchmark_write(struct BenchmarkOpts *o) {
  int file = open(o->io, O_CREAT | O_WRONLY, 0666);
  if (!file) {
    printf("Error opening source file '%s': %s\n", o->io, strerror(errno));
  }
  char buf[o->page_size];
  ull total_bytes_written = 0, bytes_written;
  ull start = millis();
  // random string
  while (total_bytes_written < o->bytes) {
    bytes_written = write(file, buf, o->page_size);
    if (bytes_written < 0 && errno != EAGAIN) {
      fprintf(stderr, "write error: %s\n", strerror(errno));
      exit(1);
    }
    total_bytes_written += bytes_written;
  }
  struct BenchmarkResults *results = malloc(sizeof(struct BenchmarkResults));
  results->time = millis() - start;
  results->bytes = total_bytes_written;
  results->opts = o;
  return results;
}