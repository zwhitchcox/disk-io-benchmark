
#ifndef __BENCHMARK__
#define __BENCHMARK__

#define KB 1024l
#define MB (KB*KB)
#define GB (MB*KB)
#define TB (GB*MB)


typedef struct BenchmarkOptions {
  int open_flags;
  int page_size;
  char *file;
  long bytes;
} benchmark_options_t;

struct BenchmarkResults *benchmark_read(struct BenchmarkOptions o);
struct BenchmarkResults *benchmark_write(struct BenchmarkOptions o);

#endif /* __BENCHMARK__ */