#include "common.h"

#ifndef __BENCHMARK_RESULTS__
#define __BENCHMARK_RESULTS__
void sprint_size(char *s, ull bytes, int precision);

void print_results(struct BenchmarkOptions *o, struct BenchmarkResults *results);
#endif /* __BENCHMARK_RESULTS__ */