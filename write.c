// #include <stdio.h>
// #include <fcntl.h>
// #include <errno.h>
// #include <string.h>
// #include <stdlib.h>
// #include <unistd.h>

// #define PAGE_SIZE 4096


// int benchmark_write(char *src_path, char *des_path) {
//   int src = open(src_path, O_RDONLY, 0666);
//   if (!src) {
//     printf("Error opening source file '%s': %s\n", src_path, strerror(errno));
//   }
//   int des = open(src_path, O_WRONLY | O_CREAT, 0666);
//   if (!des) {
//     printf("Error opening destination file '%s': %s\n", des_path, strerror(errno));
//   }
//   char *buf = malloc(PAGE_SIZE);
//   for (int i = 0; i < PAGE_SIZE; i++) {
//     buf[i] = 65 + (i % 26);
//   }
//   int j = 0;
//   // 4GB
//   for (int i = 0; i < 1024*1024; i++) {
//     // if (((i * 4096) % (1024*1024*1024)) == 0) {
//     //   printf("1GB\n");
//     // }
//     if (!write(f, buf, PAGE_SIZE)) {
//       printf("%s\n", strerror(errno));
//     }
//   }

//   close(f);
// }