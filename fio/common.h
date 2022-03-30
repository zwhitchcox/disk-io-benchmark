#ifndef __BENCHMARK__COMMON__
#define __BENCHMARK__COMMON__

typedef unsigned long long ull;

// so we know to remove print statement eventually
#define debug printf

// bytes
#define KB 1024ull
#define MB (KB*KB)
#define GB (MB*KB)
#define TB (GB*KB)
#define PB (TB*KB)

#define KB_str "KB"
#define MB_str "MB"
#define GB_str "GB"
#define TB_str "TB"
#define PB_str "PB"


// time
#define ms 1000ull

#ifndef O_DIRECT
#define O_DIRECT 16348
#endif


typedef struct BenchmarkOpts {
  ull page_size;
  char *io;
  char *buf;
  ull bytes;
} benchmark_opts_t;

typedef struct BenchmarkResults {
  ull time;
  ull bytes;
  struct BenchmarkOpts *opts;
} benchmark_results_t;

#endif /* __BENCHMARK__COMMON__ */