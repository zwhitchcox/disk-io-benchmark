#define _GNU_SOURCE
#include "common.h"
#include "benchmark.h"
#include "results.h"

#define POOL_SIZE 512

ull millis() {
  struct timespec _t;
  clock_gettime(CLOCK_REALTIME, &_t);
  return _t.tv_sec * 1000 + lround(_t.tv_nsec / 1e6);
}

#define STACK_SIZE 4096
static void *benchmark_copy_thread(void *arg) {
  struct ThreadInfo *ti = arg;
  int ifd;
  if ((ifd = open(ti->input_path, O_RDONLY | O_NOATIME, 0666)) == -1) {
    errExit("Error opening input: '%s'\n", ti->input_path);
  }
  int ofd;
  if ((ofd = open(ti->output_path, O_CREAT | O_WRONLY | O_TRUNC, 0666)) == -1) {
    errExit("Error opening output: '%s', %s:%d\n", ti->output_path, __FILE__, __LINE__);
  }
  char *read_buf = malloc(ti->page_size);
  off_t offset;
  int bytes_read, cur_read, bytes_written;
  do {
    pthread_mutex_lock(&ti->offset_lock);
    offset = ti->offset;
    ti->offset += ti->page_size;
    pthread_mutex_unlock(&ti->offset_lock);
    lseek(ifd, offset, SEEK_SET);
    lseek(ofd, offset, SEEK_SET);

    // This won't work with direct IO I don't think, because it won't be aligned
    bytes_read = 0;
    do {
      bytes_read += (cur_read = read(ifd, read_buf+bytes_read, ti->page_size - bytes_read));
      if (cur_read == -1) {
        errExit("benchmark_copy_thread: Read error");
      }
    } while (bytes_read < ti->page_size && cur_read);

    bytes_written = 0;
    while (bytes_written < bytes_read) {
      if (bytes_written == -1) {
        errExit("benchmark_copy_thread: Write error");
      }
      bytes_written += write(ofd, read_buf+bytes_written, bytes_read);
    }
  } while (bytes_written);

  if (close(ofd) == -1) {
    errExit("close");
  }
  if (close(ifd) == -1) {
    errExit("close");
  }
  return NULL;
}

struct BenchmarkResults *benchmark_copy(struct BenchmarkOptions *o) {
  thread_info *ti = malloc(sizeof(thread_info));
  ti->input_path = o->input_path;
  ti->output_path = o->output_path;
  ti->offset = 0;
  ti->page_size = o->page_size;
  if (pthread_mutex_init(&ti->offset_lock, NULL) != 0) {
    errExit("mutex init failed");
  };

  struct BenchmarkResults *results = malloc(sizeof(struct BenchmarkResults));
  results->start = millis();

  int s;
  pthread_t *threads = calloc(o->num_threads, sizeof(pthread_t));
  for (int i = 0; i < o->num_threads; i++) {
    s = pthread_create(&threads[i], NULL, &benchmark_copy_thread, ti);
    if (s != 0) {
      errExitEN(s, "pthread_create\n");
    }
  }

  void *res;
  for (int i = 0; i < o->num_threads; i++) {
    s = pthread_join(threads[i], &res);
    if (s != 0) {
      errExitEN(s, "pthread_join");
    }
    printf("Joined with thread %ld; returned value was  %s\n",
      threads[i], (char *) res);
    free(res);
  }
  results->bytes = ti->offset;
  results->end = millis();
  free(threads);
  free(ti);
  return results;
}