#include "benchmark.h"

#include <errno.h>
#include <fcntl.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include "common.h"
#include "results.h"

ull millis() {
  struct timespec _t;
  clock_gettime(CLOCK_REALTIME, &_t);
  return _t.tv_sec * 1000 + lround(_t.tv_nsec / 1e6);
}

struct BenchmarkResults *benchmark_write(struct BenchmarkOpts *o) {
  int file = open(o->io, O_CREAT | O_WRONLY, 0666);
  if (!file) {
    printf("Error opening source file '%s': %s\n", o->io, strerror(errno));
  }
  ull total_bytes_written = 0, bytes_written;
  ull start = millis();
  // random string
  while (total_bytes_written < o->bytes) {
    ull remainder = total_bytes_written % o->page_size;
    bytes_written = write(file, o->buf, o->page_size - remainder);
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
  close(file);
  return results;
}

struct BenchmarkResults *benchmark_read(struct BenchmarkOpts *o) {
  int file;
  if ((file = open(o->io, O_RDONLY, 0666)) <= 0) {
    printf("Error opening source file '%s': %s\n", o->io, strerror(errno));
    exit(1);
  }
  char read_buf[o->page_size];
  ull total_bytes_read = 0, bytes_read;
  ull start = millis();
  long i = 0;
  while (total_bytes_read < o->bytes) {
    bytes_read = read(file, read_buf, o->page_size);
    if (bytes_read < 0 && errno != EAGAIN) {
      fprintf(stderr, "Read error: %s\n", strerror(errno));
      exit(1);
    }
    for (int j = 0; j < bytes_read; j++) {
      if (read_buf[j] != o->buf[(total_bytes_read + j) % o->page_size]) {
        fprintf(stderr, "did not match: %c, %c: %d, %llu\n", read_buf[j],
                o->buf[(total_bytes_read + j) % o->page_size],
                j, bytes_read);
        exit(1);
      }
    }
    total_bytes_read += bytes_read;
  }
  struct BenchmarkResults *results = malloc(sizeof(struct BenchmarkResults));
  results->time = millis() - start;
  results->bytes = total_bytes_read;
  results->opts = o;
  close(file);
  return results;
}