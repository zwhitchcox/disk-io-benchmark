#ifndef __BENCHMARK_DEBUG__
#define __BENCHMARK_DEBUG__

#ifndef DEBUG
#define debug(args...)
#else
#define debug printf
#endif

#endif /* __BENCHMARK_DEBUG__ */