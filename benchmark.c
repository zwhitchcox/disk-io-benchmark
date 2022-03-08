#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>

#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <linux/fs.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <sys/ioctl.h>

#define PAGE_SIZE 4096
#define TIMES_TO_WRITE 4096
#define TEST_DEV "/dev/sda"

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
  int NUM_PAGES = 8;
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
  } else {
    printf("opened disk\n");
  }
  int i = 0;
  for (; i < PAGE_SIZE; i++) {
    buf[i] = 'n';
  }
  printf("i: %d\n", i);
  printf("size: %d\n", sizeof(*buf));
  buf[20] = '\0';
  buf[21] = '\n';
  printf("%s", (char *) buf);
  // fprintf(stderr, (char *)buf, PAGE_SIZE);
  // fflush(stdout);
  printf("\n");
  if (!write(disk, buf, PAGE_SIZE)) {
    printf("Error writing: %s\n", strerror(errno));
  }

  // int num;
  // FILE *fptr;

  //  // use appropriate location if you are using MacOS or Linux
  // fptr = fopen("/dev/sda","w");

  // if(fptr == NULL)
  // {
  //    printf("Error!");
  //    exit(1);
  // }

  // printf("Enter num: ");
  // scanf("%d",&num);

  // fprintf(fptr,"%d",num);
  // fclose(fptr);
  close(disk);
  return 0;
}