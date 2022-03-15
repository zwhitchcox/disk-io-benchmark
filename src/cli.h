#include "benchmark.h"

#ifndef __BENCHMARK_CLI__
#define __BENCHMARK_CLI__

#define DEFAULT_NUM_BYTES GB
#define DEFAULT_PAGE_SIZE 512

enum BMType {
  BM_READ,
  BM_WRITE,
};

typedef struct BenchmarkOpts {
  int open_flags;
  int page_size;
  char *file;
  long num_bytes;
  enum BMType type;
} benchmark_opts_t;

#endif