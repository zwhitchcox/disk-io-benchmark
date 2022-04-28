#include <getopt.h>

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
  long num = 0;
  while (isdigit((int) *bytes)) {
    num *= 10;
    num += (*bytes++ - '0') % 10;
  }
  if (*bytes) {
    return num * text_to_multiple(bytes);
  }
  return num;
}

void print_options(benchmark_opts *o) {
  printf("input: %s\n", o->input_path);
  printf("output: %s\n", o->output_path);
  printf("num_threads: %d\n", o->num_threads);
  printf("buf_size: %ld\n", o->buf_size);
  printf("align: %ld\n", o->align);
  printf("direct: %s\n", o->direct ? "true" : "false");
}


int main(int argc, char *argv[]) {
  int opt;
  benchmark_opts *o = malloc(sizeof(benchmark_opts));
  o->buf_size = 64ull*KB;
  o->align = 4*1024;
  char *end;
  while ((opt = getopt(argc, argv, "db:j:a:")) != -1) {
    switch (opt) {
      case 'a':
        o->align = normalize_bytes(optarg);
        break;
      case 'b':
        o->buf_size = normalize_bytes(optarg);
        break;
      case 'd':
        o->direct = true;
        break;
      case 'j':
        o->num_threads = strtod(optarg, &end);
        break;
      default: /* '?' */
        fprintf(stderr, "Usage: %s [-b buffer_size] [-j num_threads] input_file output_file\n", argv[0]);
        exit(EXIT_FAILURE);
    }
  }

  o->input_path = argv[optind];
  o->output_path = argv[optind+1];
  print_options(o);

  benchmark_results *r = benchmark_copy(o);
  print_results(o, r);

  free(o);
  free(r);
}