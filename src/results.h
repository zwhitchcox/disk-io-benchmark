#ifndef __BENCHMARK_RESULTS__
#define __BENCHMARK_RESULTS__
void sprint_size(char *s, long bytes, int precision);

typedef struct BenchmarkResults {
  long bytes;
  long time;
} benchmark_results_t;

void print_results(struct BenchmarkResults *results);
#endif /* __BENCHMARK_RESULTS__ */