#define LCOM_COMPAT_IMPLEMENTATION
#include <lcom/compat.h>
#include "output_log.h"

#include <fcntl.h>
#include <poll.h>
#include <pthread.h>
#include <sys/ioctl.h>
#include <sys/stat.h>

struct output_log {
  int input, writer, stop_reader, stop_writer, forward;
  FILE *file;
  pthread_t thread;
  int error;
};
static struct output_log *active;
static pthread_mutex_t configuration = PTHREAD_MUTEX_INITIALIZER;
static bool exit_registered;

static int write_all(int fd, const char *bytes, size_t size) {
  while (size) {
    ssize_t written = write(fd, bytes, size);
    if (written < 0) { if (errno == EINTR) continue; return errno; }
    if (!written) return EIO;
    bytes += written; size -= (size_t)written;
  }
  return 0;
}

static void *copy_output(void *argument) {
  struct output_log *log = argument;
  char bytes[4096];
  int forward_error = 0, file_error = 0;
  struct pollfd descriptors[] = {{log->input, POLLIN, 0}, {log->stop_reader, POLLIN, 0}};
  for (;;) {
    int ready = poll(descriptors, 2, -1);
    if (ready < 0) { if (errno == EINTR) continue; log->error = errno; break; }
    bool stopping = (descriptors[1].revents & (POLLIN | POLLHUP)) != 0;
    int remaining = 0;
    if (stopping && ioctl(log->input, FIONREAD, &remaining) < 0) { log->error = errno; break; }
    // Drain only the bytes present at cleanup. Other inherited writers must
    // neither keep cleanup waiting for EOF nor extend it with new output.
    do {
      size_t size = sizeof(bytes);
      if (stopping) {
        if (!remaining) break;
        if ((size_t)remaining < size) size = (size_t)remaining;
      }
      ssize_t received = read(log->input, bytes, size);
      if (received < 0) {
        if (errno == EINTR) continue;
        if (errno != EAGAIN) log->error = errno;
        break;
      }
      if (!received) { stopping = true; break; }
      if (!forward_error) forward_error = write_all(log->forward, bytes, (size_t)received);
      if (!file_error) file_error = write_all(fileno(log->file), bytes, (size_t)received);
      if (stopping) remaining -= (int)received;
    } while (stopping && remaining);
    if (stopping || log->error) break;
    if ((descriptors[0].revents | descriptors[1].revents) & (POLLERR | POLLNVAL)) { log->error = EIO; break; }
  }
  if (!log->error) log->error = forward_error ? forward_error : file_error;
  return NULL;
}

static int close_log(struct output_log *log, bool restore) {
  int error = 0;
  if (restore) {
    flockfile(stdout);
    if (fflush(stdout)) error = errno;
    while (dup2(log->forward, STDOUT_FILENO) < 0) {
      if (errno == EINTR) continue;
      if (!error) error = errno;
      break;
    }
  }
  if (log->writer >= 0) close(log->writer);
  int stopped = write_all(log->stop_writer, "x", 1);
  int joined = pthread_join(log->thread, NULL);
  if (!error) error = stopped;
  if (!error) error = joined;
  if (!error) error = log->error;
  if (restore) funlockfile(stdout);
  close(log->input); close(log->stop_reader); close(log->stop_writer); close(log->forward);
  if (fclose(log->file) && !error) error = errno;
  free(log);
  return error;
}

int lcom_compat_output_log_stop(void) {
  pthread_mutex_lock(&configuration);
  struct output_log *log = active; active = NULL;
  int error = log ? close_log(log, true) : 0;
  pthread_mutex_unlock(&configuration);
  if (error) errno = error;
  return error;
}

static void stop_at_exit(void) { (void)lcom_compat_output_log_stop(); }

static int descriptor_flags(int descriptor, bool nonblocking) {
  if (fcntl(descriptor, F_SETFD, FD_CLOEXEC) < 0) return errno;
  if (nonblocking) {
    int flags = fcntl(descriptor, F_GETFL);
    if (flags < 0 || fcntl(descriptor, F_SETFL, flags | O_NONBLOCK) < 0) return errno;
  }
  return 0;
}

static struct output_log *prepare_log(const char *path) {
  FILE *file = lcom_compat_fopen(path, "a");
  if (!file) return NULL;
  struct output_log *log = calloc(1, sizeof(*log));
  if (!log) { int error = errno; fclose(file); errno = error; return NULL; }
  log->file = file; log->input = log->writer = log->stop_reader = log->stop_writer = log->forward = -1;
  int data[2], control[2], error = 0;
  if (pipe(data)) error = errno;
  else {
    log->input = data[0]; log->writer = data[1];
    if (pipe(control)) error = errno;
    else { log->stop_reader = control[0]; log->stop_writer = control[1]; }
  }
  if (!error) {
    log->forward = dup(active ? active->forward : STDOUT_FILENO);
    if (log->forward < 0) error = errno;
  }
  int descriptors[] = {log->input, log->writer, log->stop_reader, log->stop_writer, log->forward};
  for (size_t i = 0; !error && i < sizeof(descriptors) / sizeof(descriptors[0]); ++i)
    error = descriptor_flags(descriptors[i], i == 0);
  if (!error) error = pthread_create(&log->thread, NULL, copy_output, log);
  if (!error) return log;
  for (size_t i = 0; i < sizeof(descriptors) / sizeof(descriptors[0]); ++i) if (descriptors[i] >= 0) close(descriptors[i]);
  fclose(file); free(log); errno = error;
  return NULL;
}

bool lcf_log_output(const char *path) {
  if (!path) { errno = EINVAL; return false; }
  pthread_mutex_lock(&configuration);
  if (!exit_registered) {
    if (atexit(stop_at_exit)) { pthread_mutex_unlock(&configuration); errno = ENOMEM; return false; }
    exit_registered = true;
  }
  struct output_log *next = prepare_log(path);
  if (!next) { pthread_mutex_unlock(&configuration); return false; }
  flockfile(stdout);
  struct output_log *previous = active; active = NULL;
  int error = previous ? close_log(previous, true) : 0;
  struct stat info;
  if (!error && fstat(fileno(next->file), &info)) error = errno;
  // Open without truncating first, so a failed open preserves an active log.
  // Finish the previous worker before truncating, including for the same path.
  if (!error && S_ISREG(info.st_mode) && ftruncate(fileno(next->file), 0)) error = errno;
  if (!error) {
    flockfile(stdout);
    if (fflush(stdout)) error = errno;
    if (!error && dup2(next->writer, STDOUT_FILENO) < 0) error = errno;
    funlockfile(stdout);
  }
  if (!error) { close(next->writer); next->writer = -1; active = next; }
  else (void)close_log(next, false);
  funlockfile(stdout);
  pthread_mutex_unlock(&configuration);
  if (error) errno = error;
  return error == 0;
}
