#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <math.h>

#include "results.h"
#include "helpers.h"
#include "benchmark.h"

// integer decimal point round
long idecround(long num, long len) {
  int precision = log10(num);
  if (precision < len) {
    return num;
  }
  return num / (precision-len);
}

struct OpenFlags {
  char *name;
  int num;
};

#define open_flag_str(OPT) (struct OpenFlags) { \
    .name = #OPT, \
    .num = OPT, \
  }


struct OpenFlags open_flags_map[] = {
  open_flag_str(O_DIRECT),
  open_flag_str(O_CREAT),
  open_flag_str(O_RDONLY),
  open_flag_str(O_RDWR),
};



void print_open_flags(long flags) {
  struct OpenFlags ofm;
  printf("open flags: ");
  if (!O_RDONLY && !flags) {
    printf("O_RDONLY\n") ;
    return;
  }
  for (int i = 0; i < sizeof(open_flags_map) / sizeof(open_flags_map[0]); i++) {
    ofm = open_flags_map[i];
    // printf("checking %s\n", ofm.name);
    // printf("%d\n", flags);
    if (ofm.num & flags) {
      printf("%s ", ofm.name);
    }
  }
  printf("\n");
}

// print size in KB, MB, or GB
void sprint_size(char *s, long bytes, int precision) {
  char *unit;
  long denominator = 1;
  if (bytes <= KB) {
    unit = "B";
  } else if (bytes < MB) {
    unit = "KB";
    denominator = KB;
  } else if (bytes < GB) {
    unit = "MB";
    denominator = MB;
  } else if (bytes < TB) {
    denominator = GB;
    unit = "GB";
  } else {
    fprintf(stderr, "size too large.");
    exit(1);
  }
  long dec = idecround(bytes % denominator, precision);
  s += sprintf(s, "%ld", bytes / denominator);
  if (dec > 0) {
    s += sprintf(s, ".%ld", dec);
  }
  s += sprintf(s, "%s", unit);
}

void print_speed(long bytes, long ms) {
  int precision = 1000;
  long mbpu = bytes*precision / MB * 1000 / ms;
  printf("MB/s: %ld.%ld\n", mbpu / precision, idecround(mbpu % precision, 3));
}

void print_results(struct BenchmarkResults *results) {
  char s[1000];
  printf("time: %ld\n", results->time);
  sprint_size(s, results->bytes, 3);
  printf("bytes: %s\n", s);
  printf("MB/s: %ld\n", results->bytes * 1000  / MB / results->time);
}