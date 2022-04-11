#ifndef __BENCHMARK__
#define __BENCHMARK__
#include "common.h"
#include <sys/types.h>

typedef struct ThreadInfo {
  pthread_mutex_t offset_lock;
  off_t offset;
  char *output_path;
  char *input_path;
  off_t page_size;
} thread_info;

struct BenchmarkResults *benchmark_read(struct BenchmarkOptions *o);
struct BenchmarkResults *benchmark_write(struct BenchmarkOptions *o);
struct BenchmarkResults *benchmark_copy(struct BenchmarkOptions *o);

#endif /* __BENCHMARK__ */