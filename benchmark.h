#ifndef O_DIRECT
#define O_DIRECT 16348
#endif

enum BMType {
  BM_READ,
  BM_WRITE,
};

typedef struct BenchmarkOptions {
  int open_flags;
  int page_size;
  char *file;
  long num_bytes;
  enum BMType type;
} benchmark_options_t;

int benchmark_write(struct BenchmarkOptions *o);