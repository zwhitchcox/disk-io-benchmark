#ifndef O_DIRECT
#define O_DIRECT 16348
#endif

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
  long num_bytes;
} benchmark_options_t;

#endif /* __BENCHMARK__ */


// enum BMType {
//   BM_READ = 1 << 1,
//   BM_WRITE = 1 << 2,
// };
// int benchmark_write(struct BenchmarkOptions *o);