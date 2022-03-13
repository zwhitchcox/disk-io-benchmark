#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <time.h>

#include <unistd.h>
#include <linux/fs.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <sys/ioctl.h>

#define _GNU_SOURCE
#define PAGE_SIZE 4096
#define TEST_DEV "./sda"

// 64 MB
#define NUM_PAGES 10*1024*1024

void printbuf(int *buf) {
  for (int i = 0; i < PAGE_SIZE; i++) {
    printf("%d\n", buf[i]);
  }
}

#ifndef O_DIRECT
#define O_DIRECT 16348
#endif

int main() {
  // int *p1 = malloc(10*sizeof *p1);
  // printf("default-aligned addr:   %p\n", (void*)p1);
  // free(p1);

  // int *p2 = aligned_alloc(1024, 1024*sizeof *p2);
  // printf("1024-byte aligned addr: %p\n", (void*)p2);
  // free(p2);
  char *buf = NULL;
  int err;
  if (err = posix_memalign((void **) &buf, PAGE_SIZE, PAGE_SIZE)) {
    printf("Error aligning memory: %s\n", strerror(err));
  }

  // int *buf = NULL;
  // if (!(buf = aligned_alloc(PAGE_SIZE*sizeof(buf)*100, PAGE_SIZE*sizeof(*buf)))) {
  //   printf("Error aligning memory: %s\n", strerror(errno));
  // }
  int disk = open(TEST_DEV, O_RDWR);
  if (!disk) {
    printf("Error opening disk: %s\n", strerror(errno));
  }
  int i = 0;
  struct timeval seed;
  gettimeofday(&seed, NULL);
  srand(seed.tv_usec);
  int rand_char = 97 + rand() % 26;

  for (; i < PAGE_SIZE; i++) {
    buf[i] = rand_char;
  }
  printf("GB: %d\n", (NUM_PAGES/1024/1024)*(PAGE_SIZE/1024));
  clock_t begin = clock();
  for (long long int i = 0; i < NUM_PAGES; i++) {
    if (!write(disk, buf, PAGE_SIZE)) {
      printf("Error writing: %s\n", strerror(errno));
    }
  }
  clock_t end = clock();
  double time_spent = (double) (end - begin) / CLOCKS_PER_SEC;
  printf("write time: %f seconds\n", time_spent);
  lseek(disk, 0, SEEK_SET);
  begin = clock();
  for (int i = 0; i < NUM_PAGES; i++) {
    if (!read(disk, buf, PAGE_SIZE)) {
      printf("Error writing: %s\n", strerror(errno));
    } else {
      for (int j = 0; j < PAGE_SIZE; j++) {
        if (buf[j] != rand_char) {
          printf("i: %d, bad char: %d, got %d instead of  %d\n", i, j, buf[j], rand_char);
        }
      }
    }
  }
  end = clock();
  time_spent = (double) (end - begin) / CLOCKS_PER_SEC;
  printf("read time: %f seconds\n", time_spent);
  close(disk);
  return 0;
}