#define _GNU_SOURCE
#include "common.h"
#include "benchmark.h"
#include "results.h"
#include <unistd.h>

ull millis() {
  struct timespec _t;
  clock_gettime(CLOCK_REALTIME, &_t);
  return _t.tv_sec * 1000 + lround(_t.tv_nsec / 1e6);
}

static void *benchmark_copy_thread(void *arg) {
  thread_info *ti = arg;
  benchmark_opts *o = ti->opts;

  int iflags = O_RDONLY | O_NOATIME;
  if (o->direct) {
    iflags |= O_DIRECT;
  }
  int ifd;
  if ((ifd = open(o->input_path, iflags, 0666)) == -1) {
    errExit("Error opening input: '%s'\n", o->input_path);
  }

  int oflags = O_CREAT | O_WRONLY;
  if (o->direct) {
    oflags |= O_DIRECT;
  }
  int ofd;
  if ((ofd = open(o->output_path, oflags, 0666)) == -1) {
    errExit("Error opening output: '%s'\n", o->output_path);
  }
  off_t buf_size = o->buf_size - (o->buf_size % o->align); // size must be aligned too

  char *buf;
  if (o->direct) {
    if (posix_memalign((void **) &buf, o->align, buf_size)) {
      errExit("posix_memalign");
    }
  } else {
    buf = malloc(buf_size);
  }

  off_t offset;
  int bytes_read, cur_read;
  int bytes_written = 0;
  do {
    pthread_mutex_lock(&ti->offset_lock);
    offset = ti->offset;
    ti->offset += buf_size;
    pthread_mutex_unlock(&ti->offset_lock);

    if (o->direct) {
      bytes_read = 0;
      do {
        lseek(ifd, offset, SEEK_SET);
        bytes_read = read(ifd, buf, buf_size);
        if (bytes_read < o->buf_size) {
          if (!read(ifd, buf, buf_size)) {
            break;
          }
          if (bytes_read == -1) {
            errExit("benchmark_copy_thread: read error");
          }
        }
      } while (bytes_read < buf_size);

      bytes_written = 0;
      while (bytes_written < bytes_read) {
        lseek(ofd, offset+bytes_written, SEEK_SET);
        bytes_written = write(ofd, buf, bytes_read);
        bytes_read-= bytes_written;
        if (bytes_written == -1) {
          errExit("benchmark_copy_thread: write error");
        }
      };
    } else {
      bytes_read = 0;
      lseek(ifd, offset, SEEK_SET);
      lseek(ofd, offset, SEEK_SET);
      do {
        bytes_read += (cur_read = read(ifd, buf + bytes_read, buf_size-bytes_read));
        if (bytes_read < buf_size) {
          if (cur_read == -1) {
            errExit("benchmark_copy_thread: Read error");
          }
        }
      } while (bytes_read < buf_size && cur_read);

      bytes_written = 0;
      while (bytes_written < bytes_read) {
        bytes_written += write(ofd, buf + bytes_written, bytes_read);
        if (bytes_written < bytes_read) {
          errExit("benchmark_copy_thread: Write error");
        }
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