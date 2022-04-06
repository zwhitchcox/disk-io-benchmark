#ifndef __BENCHMARK__
#define __BENCHMARK__
#include "common.h"

struct BenchmarkResults *benchmark_read(struct BenchmarkOptions *o);
struct BenchmarkResults *benchmark_write(struct BenchmarkOptions *o);
struct BenchmarkResults *benchmark_copy(struct BenchmarkOptions *o);

#endif /* __BENCHMARK__ */