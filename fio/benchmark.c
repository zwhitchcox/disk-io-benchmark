#include "benchmark.h"

#include <errno.h>
#include <fcntl.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <stdbool.h>
#include <pthread.h>

#include "common.h"
#include "results.h"

#define POOL_SIZE 512

ull millis() {
  struct timespec _t;
  clock_gettime(CLOCK_REALTIME, &_t);
  return _t.tv_sec * 1000 + lround(_t.tv_nsec / 1e6);
}

struct BenchmarkResults *benchmark_copy(struct BenchmarkOptions *o) {
  int input_file;
  if ((input_file = open(o->input, O_RDONLY, 0666)) <= 0) {
    printf("Error opening source '%s': %s\n", o->input, strerror(errno));
    exit(1);
  }
  int output_file;
  if ((output_file = open(o->output, O_CREAT | O_WRONLY | O_TRUNC, 0666)) <= 0) {
    printf("Error opening output '%s': %s\n", o->output, strerror(errno));
    exit(1);
  }
  char read_buf[o->page_size];
  ull total_bytes_written = 0;
  int bytes_written, bytes_read;
  ull start = millis();
  do {
    bytes_read = read(input_file, read_buf, o->page_size);
    if (bytes_read < 0 && errno != EAGAIN) {
      fprintf(stderr, "Read error: %s\n", strerror(errno));
      exit(1);
    }
    bytes_written = 0;
    while (bytes_written < bytes_read) {
      if (bytes_written < 0 && errno != EAGAIN) {
        fprintf(stderr, "write error: %s\n", strerror(errno));
        exit(1);
      }
      bytes_written = write(output_file, read_buf, bytes_read);
      bytes_read -= bytes_written;
      total_bytes_written += bytes_written;
    }
  } while (bytes_written);

  struct BenchmarkResults *results = malloc(sizeof(struct BenchmarkResults));
  results->time = millis() - start;
  results->bytes = total_bytes_written;
  close(output_file);
  close(input_file);
  return results;
}

#define STACK_SIZE 4096
void benchmark_copy_thread(struct BenchmarkOptions *o, pthread_mutex_t mutex) {

}

struct BenchmarkResults *benchmark_copy_threaded(struct BenchmarkOptions *o) {
  pthread_mutex_t mutex;
  pthread_mutex_init(&mutex, NULL);

}