#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <malloc.h>

#define PAGE_SIZE 4096
#define PAGE_PRINTS 1

void printbuf(int *buf) {
  for (int i = 0; i < PAGE_SIZE; i++) {
    printf("%d\n", buf[i]);
  }
}

int main() {
  int *buf = NULL;
  if (posix_memalign((void **) &buf, PAGE_SIZE*2, PAGE_SIZE*2)) {
    printf("Error aligning memory: %s\n", strerror(errno));
  }
  for (int i = 0; i < PAGE_SIZE; i++) {
    buf[i] = 57;
  }
  printbuf(buf);
}