#include <stdio.h>
#include <stdlib.h>

#include "cli.h"

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


int main(int argc, char *argv[]) {
  if (argc > 2) {
    fprintf(stderr, "only one file argument allowed at this time.\n");
    exit(1);
  }
  benchmark(argv[1]);
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