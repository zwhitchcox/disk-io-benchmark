#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <math.h>

#include "benchmark.h"


#define KB 1024l
#define MB (KB*KB)
#define GB (MB*KB)

#define DEFAULT_NUM_BYTES GB
#define DEFAULT_PAGE_SIZE 512


long millis(){
    struct timespec _t;
    clock_gettime(CLOCK_REALTIME, &_t);
    return _t.tv_sec*1000 + lround(_t.tv_nsec/1e6);
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
  for (int i = 0; i < sizeof(open_flags_map) / sizeof(open_flags_map[0]); i++) {
    ofm = open_flags_map[i];
    if (ofm.num & flags) {
      printf("%s ", ofm.name);
    }
  }
    printf("\n");
}

int benchmark_read(struct BenchmarkOptions *o) {
  int file = open(o->file, o->open_flags, 0666);
  if (!file) {
    printf("Error opening source file '%s': %s\n", o->file, strerror(errno));
  }
  char buf[o->page_size];
  long total_bytes_read = 0, bytes_read;
  long start = millis();
  int i = 0;
  while (total_bytes_read < o->num_bytes) {
    bytes_read = read(file, buf, o->page_size);
    if (bytes_read < 0 && errno != EAGAIN) {
      fprintf(stderr, "%s\n", strerror(errno));
      return 1;
    }
    total_bytes_read += bytes_read;
    if (!i) {
      printf("%s\n", buf);
    }
    i++;
  }
  long end = millis();
  long time = end - start;
  printf("Time: %ld.%ld\n", time / 1000, time % 1000);
  printf("MB: %ld\n", total_bytes_read / MB);
  printf("MB/s: %ld.%ld\n", (total_bytes_read * 1000l / MB / time), (total_bytes_read * 1000l % (MB * time)) / 1000000);
  // int des = open(o->des, o->w_open_flags, 0666);
  // if (!des) {
  //   printf("Error opening destination file '%s': %s\n", o->des, strerror(errno));
  // }
}

int benchmark_write(struct BenchmarkOptions *o) {
  int file = open(o->file, o->open_flags, 0666);
  if (!file) {
    printf("Error opening source file '%s': %s\n", o->file, strerror(errno));
  }
  char buf[o->page_size];
  long total_bytes_read = 0, bytes_read;
  long start = millis();
  int i = 0;
  // random string
  char *hello = "hello";
  int len = strlen(hello);
  for (int i = 0; i < o->page_size; i++) {
    buf[i] = hello[i % len];
  }
  while (total_bytes_read < o->num_bytes) {
    bytes_read = write(file, buf, o->page_size);
    if (bytes_read < 0 && errno != EAGAIN) {
      fprintf(stderr, "%s\n", strerror(errno));
      return 1;
    }
    total_bytes_read += bytes_read;
    if (!i) {
      printf("%s\n", buf);
    }
    i++;
  }
  long end = millis();
  long time = end - start;
  printf("Time: %ld.%ld\n", time / 1000, time % 1000);
  printf("MB: %ld\n", total_bytes_read / MB);
  printf("MB/s: %ld.%ld\n", (total_bytes_read * 1000l / MB / time), (total_bytes_read * 1000l % (MB * time)) / 1000000);
}

struct BenchmarkOptions bm1 = {
  .type = BM_READ,
  .open_flags = O_RDONLY,
  .file = "/dev/sda",
};
struct BenchmarkOptions bm2 = {
  .type = BM_READ,
  .open_flags = O_RDONLY,
  .file = "./test.txt",
  // .page_size = 4096,
};
struct BenchmarkOptions bm3 = {
  .type = BM_WRITE,
  .open_flags = O_WRONLY | O_CREAT,
  .file = "/dev/sda",
};


struct BenchmarkOptions *benchmarks[] = {&bm1, &bm2, &bm3};

int main(int argc, char *argv[]) {
  print_open_flags(O_DIRECT | O_CREAT);
  return 0;
  char *file = argv[1];
  for (int i = 0; i < sizeof(benchmarks) / sizeof(benchmarks[0]); i++) {
    struct BenchmarkOptions *bm = benchmarks[i];
    if (!bm->file) {
      bm->file = file;
    }
    if (!bm->page_size) {
      bm->page_size = DEFAULT_PAGE_SIZE;
    }
    if (!bm->num_bytes) {
      bm->num_bytes = DEFAULT_NUM_BYTES;
    }
    if (bm->type == BM_READ) {
      benchmark_read(bm);
    } else if (bm->type == BM_WRITE) {
      benchmark_write(bm);
    }
  }
}