#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <fcntl.h>
#include <pthread.h>

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

char *get_buf(struct BenchmarkOpts *o) {
  return malloc(sizeof(char) * o->page_size);
}

void fill_buf(struct BenchmarkOpts *o, char *str) {
  int len = strlen(str);
  for (int i = 0; i < o->page_size; i++) {
    o->buf[i] = str[i % len];
  }
}

int main(int argc, char *argv[]) {
  char *files[MAX_FILES];
  char *copy_files[MAX_FILES];
  char **cur_file = files;
  char **cur_copy_files =  copy_files;
  int num_files = 0;
  ull bytes = 0;
  int page_size = 0;
  for (int i = 1; i < argc; i++) {
    if (!strcmp(argv[i], "-b")) {
      bytes = normalize_bytes(argv[++i]);
    } else if (!strcmp(argv[i], "-p")) {
      page_size = normalize_bytes(argv[++i]);
    } else if (!strcmp(argv[i], "-c")) {
      *cur_copy_files++ = argv[++i];
      *cur_copy_files++ = argv[++i];
    } else {
      *cur_file++ = argv[i];
    }
  }

  page_size = page_size ? page_size : 64ull*KB;
  bytes = bytes ? bytes : 512ull*MB;


  if (cur_file == files && cur_copy_files == copy_files) {
    fprintf(stderr, "No files specified\n");
    exit(1);
  }

  cur_file = files;
  cur_copy_files = copy_files;
  struct BenchmarkOpts *o = malloc(sizeof(struct BenchmarkOpts));
  o->bytes = bytes;
  o->io = *cur_file;
  o->page_size = page_size;
  o->buf = get_buf(o);
  fill_buf(o, "hello world, how are you.");

  while (*cur_file) {
    printf("file: %s\n", *cur_file);
    o->io = *cur_file++;
    printf("WRITE\n");
    print_results(benchmark_write(o));
    printf("\nREAD\n");
    print_results(benchmark_read(o));
  }
  while (*cur_copy_files) {
    printf("\nCOPY\n");
    o->input = *cur_copy_files++;
    o->output = *cur_copy_files++;
    print_results(benchmark_copy(o));
  }
}