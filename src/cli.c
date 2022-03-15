#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <fcntl.h>

#include "cli.h"
#include "benchmark.h"

#define DEFAULT_PAGE_SIZE 512
#define MAX_FILES 1024

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

// int get_bool_char_arg(int argc, char *argv[] char c) {
//   for (int i = 1; i < argc; i++) {
//     if (*argv[i] == '-' && ) {
//       return argv[i+1];
//     }
//   }
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

long normalize_bytes(char *bytes) {
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
  char *files[MAX_FILES];
  char **cur_file = files;
  long read_bytes = 0;
  long write_bytes = 0;
  for (int i = 1; i < argc; i++) {
    if (!strcmp(argv[i], "-r")) {
      read_bytes = normalize_bytes(argv[++i]);
    } else if (!strcmp(argv[i], "-w")) {
      write_bytes = normalize_bytes(argv[++i]);
    }else if (!strcmp(argv[i], "-rw") || !strcmp(argv[i], "-wr")) {
      read_bytes  = normalize_bytes(argv[i]);
      write_bytes = normalize_bytes(argv[++i]);
    } else {
      *cur_file++ = argv[i];
    }
  }
  if (cur_file == files) {
    fprintf(stderr, "No files specified\n");
    exit(1);
  }
  cur_file = files;
  while (*cur_file) {
    if (write_bytes) {
      benchmark_write((struct BenchmarkOptions) {
        .bytes = write_bytes,
        .file = *cur_file,
        .page_size = DEFAULT_PAGE_SIZE,
        .open_flags = O_CREAT | O_WRONLY,
      });
    }
    if (read_bytes) {
      benchmark_read((struct BenchmarkOptions) {
        .bytes = read_bytes,
        .file = *cur_file,
        .page_size = DEFAULT_PAGE_SIZE,
        .open_flags = O_RDONLY,
      });
    }
  }
  // char *file = argv[1];
}

// int main(int argc, char *argv[]) {
  // struct BenchmarkOpts *opts = malloc(sizeof(struct BenchmarkOpts));
  // if (argc < 3) {
  //   fprintf(stderr, "not enough arguments.");
  //   exit(1);
  // }
  // char *infile = argv[1];
  // char **ofiles = malloc(sizeof(argv[0]) * argc - 1);
  // for (int i = 0; i < argc - 2; i++) {
  //   ofiles[i] = argv[i+2];
  // }
  // ofiles[argc-1] = NULL;
  // char **cur = ofiles;
// }