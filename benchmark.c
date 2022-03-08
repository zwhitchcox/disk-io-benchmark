#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <time.h>

#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <linux/fs.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <sys/ioctl.h>

#define PAGE_SIZE 4096
#define TEST_DEV "/dev/sda"
// 16MB
#define NUM_PAGES 1024

void printbuf(int *buf) {
  for (int i = 0; i < PAGE_SIZE; i++) {
    printf("%d\n", buf[i]);
  }
}

int main() {
  // int *p1 = malloc(10*sizeof *p1);
  // printf("default-aligned addr:   %p\n", (void*)p1);
  // free(p1);

  // int *p2 = aligned_alloc(1024, 1024*sizeof *p2);
  // printf("1024-byte aligned addr: %p\n", (void*)p2);
  // free(p2);
  char *buf = NULL;
  int err;
  if (err = posix_memalign((void **) &buf, PAGE_SIZE, PAGE_SIZE*NUM_PAGES)) {
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
  for (int i = 0; i < NUM_PAGES; i++) {
    if (!write(disk, buf, PAGE_SIZE)) {
      printf("Error writing: %s\n", strerror(errno));
    }
  }
  lseek(disk, 0, SEEK_SET);
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
  close(disk);
  return 0;
}