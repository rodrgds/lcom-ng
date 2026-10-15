#include <lcom/lcf.h>
#include <sys/resource.h>
#include <signal.h>

#define CHECK(condition) do { if (!(condition)) { fprintf(stderr, "Output log check failed at line %d: %s (errno %d: %s)\n", __LINE__, #condition, errno, strerror(errno)); return 1; } } while (0)

int main(int argc, char **argv) {
  CHECK(argc >= 2);
  CHECK(setenv("LCOM_COMPAT_PROJECT_ROOT", argv[1], 1) == 0);
  char first[4096], second[4096], missing[4096], trace[4096];
  snprintf(first, sizeof(first), "%s/lab/a.log", argv[1]);
  snprintf(second, sizeof(second), "%s/b.log", argv[1]);
  snprintf(missing, sizeof(missing), "%s/missing/output.log", argv[1]);
  snprintf(trace, sizeof(trace), "%s/calls.log", argv[1]);
  if (argc > 2 && !strcmp(argv[2], "--exit")) {
    CHECK(lcf_log_output(first)); printf("flushed automatically on exit"); return 0;
  }
  if (argc > 2 && !strcmp(argv[2], "--write-failure")) {
    // Linux and MSYS provide a real failing device. Apple uses file limits.
    if (access("/dev/full", F_OK) == 0) {
      CHECK(lcf_log_output("/dev/full")); puts("console survives log write failure");
      CHECK(lcf_cleanup_fast() == ENOSPC); return 0;
    }
    CHECK(lcf_log_output(first));
    struct rlimit original, limited; CHECK(getrlimit(RLIMIT_FSIZE, &original) == 0);
    limited = original; limited.rlim_cur = 0; CHECK(signal(SIGXFSZ, SIG_IGN) != SIG_ERR);
    CHECK(setrlimit(RLIMIT_FSIZE, &limited) == 0); puts("console survives log write failure");
    int status = lcf_cleanup_fast(); CHECK(setrlimit(RLIMIT_FSIZE, &original) == 0);
    CHECK(status == EFBIG); return 0;
  }
  errno = 0; CHECK(!lcf_log_output(NULL) && errno == EINVAL);
  errno = 0; CHECK(!lcf_log_output(missing) && errno == ENOENT);
  errno = 0; CHECK(!lcf_log_output(argv[1]) && errno == EISDIR);
  // Legacy absolute paths must reach this export's private, writable asset root.
  CHECK(setenv("LCOM_COMPAT_PATH_ALIAS_COUNT", "2", 1) == 0);
  CHECK(setenv("LCOM_COMPAT_PATH_ALIAS_0_FROM", "/home/lcom/labs/legacy-team", 1) == 0);
  CHECK(setenv("LCOM_COMPAT_PATH_ALIAS_0_TO", ".", 1) == 0);
  CHECK(setenv("LCOM_COMPAT_PATH_ALIAS_1_FROM", "/home/lcom/labs/legacy-team/assets", 1) == 0);
  CHECK(setenv("LCOM_COMPAT_PATH_ALIAS_1_TO", "lab", 1) == 0);
  FILE *score = fopen("/home/lcom/labs/legacy-team/assets/score.txt", "w");
  CHECK(score); CHECK(fputs("42", score) >= 0); CHECK(fclose(score) == 0);
  score = fopen("/home/lcom/labs/legacy-team/save.txt", "w");
  CHECK(score); CHECK(fputs("17", score) >= 0); CHECK(fclose(score) == 0);
  errno = 0;
  CHECK(!fopen("/home/lcom/labs/legacy-team-other/score.txt", "w") && errno == ENOENT);
  puts("before logging");
  CHECK(lcf_log_output("/home/lcom/labs/lab/a.log"));
  printf("printf first\n"); puts("puts first"); fprintf(stdout, "fprintf first\n");
  CHECK(timer_print_elapsed_time() == 1);
  CHECK(write(STDOUT_FILENO, "write first\n", 12) == 12);
  CHECK(freopen("/dev/console", "w", stdout) == stdout); puts("console first");
  fputs("stderr stays separate\n", stderr);
  errno = 0; CHECK(!lcf_log_output(missing) && errno == ENOENT);
  puts("after failed configuration");
  CHECK(lcf_log_output(second)); puts("printf second");
  CHECK(lcf_log_output(second)); puts("printf third");
  // Cleanup must finish even if another descriptor still refers to the pipe writer.
  int held_stdout = dup(STDOUT_FILENO); CHECK(held_stdout >= 0);
  CHECK(lcf_cleanup() == OK); CHECK(close(held_stdout) == 0);
  puts("after cleanup"); CHECK(lcf_cleanup() == OK);
  errno = 0; CHECK(!lcf_trace_calls(trace) && errno == ENOSYS);
  FILE *calls = fopen(trace, "r"); CHECK(!calls && errno == ENOENT);
  return 0;
}
