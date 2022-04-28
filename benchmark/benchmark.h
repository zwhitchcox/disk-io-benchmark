#ifndef __BENCHMARK__
#define __BENCHMARK__
#include "common.h"
#include <sys/types.h>

typedef struct ThreadInfo {
  pthread_mutex_t offset_lock;
  off_t offset;
  benchmark_opts *opts;
} thread_info;

benchmark_results *benchmark_read(benchmark_opts *o);
benchmark_results *benchmark_write(benchmark_opts *o);
benchmark_results *benchmark_copy(benchmark_opts *o);

#endif /* __BENCHMARK__ */