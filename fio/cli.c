#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <fcntl.h>
#include <pthread.h>
#include <unistd.h>

#include "cli.h"
#include "benchmark.h"
#include "common.h"
#include "results.h"

// check if s1 starts with s2
int starts_with(char *s1, char *s2) {
  while (*s1 == *s2) {
    s1++;
    s2++;
  }
  return !*s2; // at the end of s2, so all of them matched
}

// char *get_char_arg(int argc, char *argv[], char c) {
//   for (int i = 1; i < argc; i++) {
//     if (*argv[i] == '-' && *(argv[i]+1) == c) {
//       return argv[i+1];
//     }
//   }
//   return NULL;
// }

// char *get_str_arg(int argc, char *argv[], char *str) {
//   for (int i = 1; i < argc; i++) {
//     if (!strcmp(argv[i], str)) {
//       return argv[i+1];
//     }
//   }
//   return NULL;
// }

enum OptionType {
  BOOLEAN,
  SINGLE_ARG,
  MULTI_ARG,
};

struct CliOption {
  enum OptionType type;
  long bytes;
};

long text_to_multiple(char *s) {
  if (!strcmp(s, "KB")) {
    return MB;
  } else if (!strcmp(s, "MB")) {
    return MB;
  } else if (!strcmp(s, "GB")) {
    return GB;
  } else {
    fprintf(stderr, "unknown denomination: %s\n", s);
    exit(1);
  }
}

ull normalize_bytes(char *bytes) {
  char *str;
  long num = 0;
  while (isdigit(*bytes)) {
    num *= 10;
    num += (*bytes++ - '0') % 10;
  }
  if (*bytes) {
    return num * text_to_multiple(bytes);
  }
  return num;
}


int main(int argc, char *argv[]) {
  int opt;
  struct BenchmarkOptions *o = malloc(sizeof(struct BenchmarkOptions));
  o->page_size = 64ull*KB;
  while ((opt = getopt(argc, argv, "p:")) != -1) {
    switch (opt) {
      case 'p':
        o->page_size = normalize_bytes(optarg);
        break;
      default: /* '?' */
        fprintf(stderr, "Usage: %s [-p pagesize] input_file output_file\n", argv[0]);
        exit(EXIT_FAILURE);
    }
  }

  if (optind != (argc - 2)) {
    fprintf(stderr, "No files specified\n");
    exit(1);
  }

  o->input = argv[optind++];
  o->output = argv[optind++];

  struct BenchmarkResults *r = benchmark_copy(o);
  print_results(o, r);

  free(o);
  free(r);
}