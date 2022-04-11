#define _GNU_SOURCE
#include "results.h"
#include "common.h"
#include "benchmark.h"

// integer decimal point round
long idecround(long num, long len) {
  int precision = log10(num);
  if (precision < len) {
    return num;
  }
  return num / (precision-len);
}

typedef struct OpenFlag {
  char *name;
  int num;
} open_flag;

#define open_flag_str(OPT) { \
    .name = #OPT, \
    .num = OPT, \
  }

open_flag open_flags_map[] = {
  open_flag_str(O_DIRECT),
  open_flag_str(O_CREAT),
  open_flag_str(O_RDONLY),
  open_flag_str(O_RDWR),
  open_flag_str(O_NONBLOCK),
  open_flag_str(O_ASYNC),
};

void print_open_flags(long flags) {
  open_flag ofm;
  printf("open flags: ");
  if (!O_RDONLY && !flags) {
    printf("O_RDONLY\n") ;
    return;
  }
  for (int i = 0; i < (int) (sizeof(open_flags_map) / sizeof(open_flags_map[0])); i++) {
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
void sprint_size(char *s, ull bytes, int precision) {
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
  ull dec = idecround(bytes % denominator, precision);
  s += sprintf(s, "%llu", bytes / denominator);
  if (dec > 0) {
    s += sprintf(s, ".%llu", dec);
  }
  s += sprintf(s, "%s", unit);
}

char *get_denomination(ull bytes) {
  switch (bytes) {
    case 1:
      return "B";
    case KB:
      return "KB";
    case MB:
      return "MB";
    case GB:
      return "GB";
    case TB:
      return "TB";
    case PB:
      return "PB";
  }
  return "[Not found: %llu]";
}

ull get_denominator(ull bytes) {
  if (bytes <= KB) {
    return 1;
  } else if (bytes < MB) {
    return KB;
  } else if (bytes < GB) {
    return MB;
  } else if (bytes < TB) {
    return GB;
  } else {
    fprintf(stderr, "size too large.");
    exit(1);
  }
}

double denominate(ull bytes) {
  return (double) bytes/(double) get_denominator(bytes);
}

void print_results(struct BenchmarkOptions *o, struct BenchmarkResults *r) {
  printf("input: %s\noutput: %s\n", o->input_path, o->output_path);
  ull time = r->end - r->start;
  printf("time: %llu.%llus\n", time / 1000, time % 1000);

  char size_res[1000];
  sprint_size(size_res, r->bytes, 3);
  printf("bytes: %s\n", size_res);

  double bps = (double) r->bytes * (double) 1000 / (double) time;
  printf("speed: %.3f%s/s\n", denominate(bps), get_denomination(get_denominator(bps)));
}