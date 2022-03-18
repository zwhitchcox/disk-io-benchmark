#include <stdio.h>
#include <xxh3.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

#define BUF_SIZE 512

XXH64_hash_t calcul_hash_streaming(int fd, char *buf, int buf_size);


int main() {
  char buf[BUF_SIZE];
  for(int i = 0; i < BUF_SIZE; i++) {
    buf[i] = 0;
  }

  // XXH64_hash_t hash = XXH64(buf, buf_size, 1123);
  int fd = open("./test.txt", O_RDONLY);
  if (fd <= 0) {
    fprintf(stderr, "Error opening file: %s", strerror(errno));
  }
  int bytes_read = 0;
  XXH64_hash_t hash = calcul_hash_streaming(fd, buf, BUF_SIZE);
  printf("%lu\n", hash);
  // read(fd, buf, BUF_SIZE);
}

XXH64_hash_t calcul_hash_streaming(int fd, char *buf, int buf_size) {
  /* create a hash state */
}