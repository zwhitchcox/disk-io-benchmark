#ifndef __BENCHMARK__
#define __BENCHMARK__
#include "common.h"

struct BenchmarkResults *benchmark_read(struct BenchmarkOpts *o);
struct BenchmarkResults *benchmark_write(struct BenchmarkOpts *o);

#endif /* __BENCHMARK__ */