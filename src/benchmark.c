#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <math.h>
#include <xxhash.h>

#include "helpers.h"
#include "benchmark.h"
#include "results.h"

long millis(){
    struct timespec _t;
    clock_gettime(CLOCK_REALTIME, &_t);
    return _t.tv_sec*1000 + lround(_t.tv_nsec/1e6);
}

struct BenchmarkResults *benchmark_read(struct BenchmarkOptions o) {
  int file;
  if ((file = open(o.file, o.open_flags, 0666)) <= 0) {
    printf("Error opening source file '%s': %s\n", o.file, strerror(errno));
    exit(1);
  }
  char buf[o.page_size];
  long total_bytes_read = 0, bytes_read;
  long start = millis();
  while (total_bytes_read < o.bytes) {
    bytes_read = read(file, buf, o.page_size);
    if (bytes_read < 0 && errno != EAGAIN) {
      fprintf(stderr, "Read error: %s\n", strerror(errno));
      exit(1);
    }
    total_bytes_read += bytes_read;
  }

  XXH64_state_t* const state = XXH64_createState();
  if (state==NULL) abort();
  struct BenchmarkResults *results = malloc(sizeof(struct BenchmarkResults));
  results->time = millis() - start;
  results->bytes = o.bytes;
  return results;

  void* const buffer = malloc(buf_size);
  if (buffer==NULL) abort();

  /* Initialize state with selected seed */
  XXH64_hash_t const seed = 100;   /* or any other value */
  if (XXH64_reset(state, seed) == XXH_ERROR) abort();

  int length = 1;
  /* Feed the state with input data, any size, any number of times */
  while (length) {
      length = read(fd, buf, buf_size);
      if (XXH64_update(state, buffer, length) == XXH_ERROR) abort();
  }

  /* Produce the final hash value */
  XXH64_hash_t const hash = XXH64_digest(state);

  /* State could be re-used; but in this example, it is simply freed  */
  free(buffer);
  XXH64_freeState(state);

  return hash;
}

struct BenchmarkResults *benchmark_write(struct BenchmarkOptions o) {
  int file = open(o.file, o.open_flags, 0666);
  if (!file) {
    printf("Error opening source file '%s': %s\n", o.file, strerror(errno));
  }
  char buf[o.page_size];
  long total_bytes_written = 0, bytes_written;
  long start = millis();
  // random string
  char *hello = "Is this actually working";
  int len = strlen(hello);
  for (int i = 0; i < o.page_size; i++) {
    buf[i] = hello[i % len];
  }
  while (total_bytes_written < o.bytes) {
    bytes_written = write(file, buf, o.page_size);
    if (bytes_written < 0 && errno != EAGAIN) {
      fprintf(stderr, "write error: %s\n", strerror(errno));
      exit(1);
    }
    total_bytes_written += bytes_written;
  }
  long end = millis();
  long time = end - start;
  struct BenchmarkResults *results = malloc(sizeof(struct BenchmarkResults));
  results->time = millis() - start;
  results->bytes = o.bytes;
  return results;
}