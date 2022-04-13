#define _GNU_SOURCE
#include "common.h"
#include "benchmark.h"
#include "results.h"
#include <linux/fs.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <sys/ioctl.h>

#define POOL_SIZE 512

ull millis() {
  struct timespec _t;
  clock_gettime(CLOCK_REALTIME, &_t);
  return _t.tv_sec * 1000 + lround(_t.tv_nsec / 1e6);
}

static void *benchmark_copy_thread(void *arg) {
  thread_info *ti = arg;
  benchmark_opts *o = ti->opts;
  int ifd;
  if ((ifd = open(o->input_path, O_RDONLY | O_NOATIME, 0666)) == -1) {
    errExit("Error opening input: '%s'\n", o->input_path);
  }

  int ofd;
  if ((ofd = open(o->output_path, O_CREAT | O_WRONLY, 0666)) == -1) {
    errExit("Error opening output: '%s'\n", o->output_path);
  }


  char *buf;
  if (o->direct) {
    if (posix_memalign((void **) &buf, o->align, o->buf_size)) {
      errExit("posix_memalign");
    }
  } else {
    buf = malloc(o->buf_size);
  }
  off_t offset;
  int bytes_read, cur_read;
  int bytes_written = 0;
  do {
    pthread_mutex_lock(&ti->offset_lock);
    offset = ti->offset;
    ti->offset += o->buf_size;
    pthread_mutex_unlock(&ti->offset_lock);
    lseek(ifd, offset, SEEK_SET);
    lseek(ofd, offset, SEEK_SET);

    // This won't work with direct IO I don't think, because it won't be aligned
    bytes_read = 0;
    do {
      bytes_read += (cur_read = read(ifd, buf + bytes_read, o->buf_size));
      if (bytes_read < o->buf_size) {
        lseek(ifd, cur_read, SEEK_CUR);
        if (cur_read == -1) {
          errExit("benchmark_copy_thread: Read error");
        }
      }
    } while (bytes_read < o->buf_size && cur_read);

    bytes_written = 0;
    while (bytes_written < bytes_read) {
      bytes_written += write(ofd, buf + bytes_written, bytes_read);
      if (bytes_written < bytes_read) {
        errExit("benchmark_copy_thread: Write error");
      }
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

benchmark_results *benchmark_copy(benchmark_opts *o) {
  thread_info *ti = malloc(sizeof(thread_info));
  ti->offset = 0;
  ti->opts = o;
  if (pthread_mutex_init(&ti->offset_lock, NULL) != 0) {
    errExit("mutex init failed");
  };

  benchmark_results *results = malloc(sizeof(benchmark_results));
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
    free(res);
  }
  results->bytes = ti->offset;
  results->end = millis();
  free(threads);
  free(ti);
  return results;
}