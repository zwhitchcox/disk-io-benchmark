#ifndef COMMON_H
#define COMMON_H

#include <sys/types.h>  /* Type definitions used by many programs */
#include <stdio.h>      /* Standard I/O functions */
#include <stdlib.h>     /* Prototypes of commonly used library functions,
                           plus EXIT_SUCCESS and EXIT_FAILURE constants */
#include <unistd.h>     /* Prototypes for many system calls */
#include <errno.h>      /* Declares errno and defines error constants */
#include <string.h>     /* Commonly used string-handling functions */
#include <stdbool.h>    /* 'bool' type plus 'true' and 'false' constants */
#include <pthread.h>
#include <math.h>
#include <fcntl.h>
#include <time.h>
#include <ctype.h>
#include "error_functions.h"  /* Declares our error-handling functions */


typedef unsigned long long ull;

// so we know to remove print statement eventually
#define debug printf

#ifdef DEBUG
#define debugvar(var, type) printf("%s: %" #type "\n", #var, var);
#else
#define debugvar
#endif


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

typedef struct BenchmarkOptions {
  char *input_path;
  char *output_path;
  off_t buf_size;
  off_t align;
  int num_threads;
  bool direct;
} benchmark_opts;

typedef struct BenchmarkResults {
  ull end;
  ull start;
  ull bytes;
} benchmark_results;

#endif /* COMMON_H */