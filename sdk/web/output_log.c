// Browser stdout is owned by Emscripten, not a POSIX pipe and copying thread.
#define LCOM_COMPAT_IMPLEMENTATION
#include <lcom/compat.h>
#include <errno.h>

bool lcf_log_output(const char *path) {
  (void)path;
  fputs("lcom-ng web: lcf_log_output is unavailable; use the browser developer console.\n", stderr);
  errno = ENOSYS;
  return false;
}
int lcom_compat_output_log_stop(void) { return 0; }
