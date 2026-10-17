/* Clang's Android format checker rejects valid C11 %n even for our wrapper. */
#if defined(__ANDROID__) && defined(__clang__)
#pragma clang diagnostic ignored "-Wformat"
#endif
#include <lcom/printf.h>
#include <assert.h>
#include <errno.h>
#include <inttypes.h>
#include <stdint.h>
#include <string.h>
#include <sys/types.h>
#include <wchar.h>
#if !defined(__EMSCRIPTEN__) && !defined(__ANDROID__)
#include <signal.h>
#include <sys/wait.h>
#include <unistd.h>
#endif

static void repeated_va_list(const char *format, ...) {
  va_list args;
  va_start(args, format);
  char first[128], second[128];
  int one = lcom_compat_vsnprintf(first, sizeof first, format, args);
  int two = lcom_compat_vsnprintf(second, sizeof second, format, args);
  va_end(args);
  assert(one == two && strcmp(first, second) == 0);
  assert(strcmp(first, "   1.25/tail") == 0);
}

int main(void) {
  char buffer[256];
  int count = -1;
  assert(lcom_compat_snprintf(buffer, sizeof buffer, "x=%04d%n/%s", 7, &count, "ok") == 9);
  assert(count == 6 && strcmp(buffer, "x=0007/ok") == 0);

  struct { signed char value; unsigned char guard[7]; } byte = {.value = -1};
  struct { short value; unsigned char guard[6]; } half = {.value = -1};
  memset(byte.guard, 0xa5, sizeof byte.guard); memset(half.guard, 0xa5, sizeof half.guard);
  long full = -1; long long wide = -1; intmax_t maximum = -1;
  ssize_t sized = -1; ptrdiff_t difference = -1;
  assert(lcom_compat_snprintf(buffer, sizeof buffer, "A%hhnB%hnC%nD%lnE%llnF%jnG%znH%tn",
                             &byte.value, &half.value, &count, &full, &wide, &maximum, &sized, &difference) == 8);
  assert(strcmp(buffer, "ABCDEFGH") == 0);
  assert(byte.value == 1 && half.value == 2 && count == 3 && full == 4 && wide == 5);
  assert(maximum == 6 && sized == 7 && difference == 8);
  for (size_t i = 0; i < sizeof byte.guard; ++i) assert(byte.guard[i] == 0xa5);
  for (size_t i = 0; i < sizeof half.guard; ++i) assert(half.guard[i] == 0xa5);

  assert(lcom_compat_snprintf(buffer, sizeof buffer, "%*.*f%n|%lld|%zu|%td|%ls",
                             8, 2, 1.25, &count, 1234567890123LL, (size_t)17, (ptrdiff_t)-9, L"wide") == 33);
  assert(count == 8 && strcmp(buffer, "    1.25|1234567890123|17|-9|wide") == 0);
  assert(lcom_compat_snprintf(buffer, sizeof buffer, "%.3Lf%n", 1.25L, &count) == 5);
  assert(count == 5 && strcmp(buffer, "1.250") == 0);
  assert(lcom_compat_snprintf(buffer, sizeof buffer, "%a%n", 1.5, &count) == 8);
  assert(count == 8 && strcmp(buffer, "0x1.8p+0") == 0);

  char reference[256];
#define INTEGER_ARGUMENTS (int)-3, (int)250, (int)-400, (int)60000, -7, 8u, -9L, 10UL, \
    -11LL, 12ULL, (intmax_t)-13, (uintmax_t)14, (ssize_t)-15, (size_t)16, (ptrdiff_t)-17, \
    (uintptr_t)18, (void *)(uintptr_t)0x1234, (wint_t)L'Q'
#define INTEGER_FORMAT "%hhd/%hhu/%hd/%hu/%d/%u/%ld/%lu/%lld/%llu/%jd/%ju/%zd/%zu/%td/%tu/%p/%lc"
  int expected = snprintf(reference, sizeof reference, INTEGER_FORMAT, INTEGER_ARGUMENTS);
  assert(lcom_compat_snprintf(buffer, sizeof buffer, INTEGER_FORMAT "%n:%s", INTEGER_ARGUMENTS, &count, "tail") == expected + 5);
  assert(count == expected && strncmp(buffer, reference, (size_t)expected) == 0);
  assert(strcmp(buffer + expected, ":tail") == 0);

  char bounded[8]; memset(bounded, 0x5a, sizeof bounded);
  assert(lcom_compat_snprintf_checked(bounded, 4, sizeof bounded, "abcdef%n:%d", &count, 7) == 8);
  assert(count == 6 && strcmp(bounded, "abc") == 0 && bounded[4] == 0x5a);
  assert(lcom_compat_snprintf(NULL, 0, "abcdef%n:%d", &count, 7) == 8 && count == 6);
  assert(lcom_compat_snprintf(bounded, 1, "abc%n", &count) == 3 && bounded[0] == 0 && count == 3);
  assert(lcom_compat_sprintf_checked(buffer, sizeof buffer, "%%n%n", &count) == 2);
  assert(count == 2 && strcmp(buffer, "%n") == 0);
  assert(lcom_compat_snprintf(buffer, sizeof buffer, "%%n") == 2 && strcmp(buffer, "%n") == 0);
  assert(lcom_compat_snprintf(buffer, sizeof buffer, "literal $ %n", &count) == 10 && count == 10);
  repeated_va_list("%*.*f%n/%s", 7, 2, 1.25, &count, "tail");

  /* Positional formatting without writeback keeps the original host path. */
  const char *positional = "%2$d/%1$d";
  assert(lcom_compat_snprintf(buffer, sizeof buffer, positional, 12, 7) == 4);
  assert(strcmp(buffer, "7/12") == 0);
  count = -1; errno = 0;
  const char *unsupported = "%1$d%2$n";
  assert(lcom_compat_snprintf(buffer, sizeof buffer, unsupported, 7, &count) == -1);
  assert(errno == ENOTSUP && count == -1);

  FILE *file = tmpfile(); assert(file);
  assert(lcom_compat_fprintf(file, "HEAD%04d%nTAIL", 7, &count) == 12 && count == 8);
  rewind(file); assert(fgets(buffer, sizeof buffer, file)); fclose(file);
  assert(strcmp(buffer, "HEAD0007TAIL") == 0);

#if !defined(__EMSCRIPTEN__) && !defined(__ANDROID__)
  for (int mode = 0; mode < 2; ++mode) {
    pid_t child = fork(); assert(child >= 0);
    if (!child) {
      (void)freopen("/dev/null", "w", stderr);
      if (mode == 0) lcom_compat_sprintf_checked(bounded, sizeof bounded, "%s", "too many bytes");
      else lcom_compat_snprintf_checked(bounded, 9, sizeof bounded, "%s", "x");
      _exit(99);
    }
    int status; assert(waitpid(child, &status, 0) == child);
    assert(WIFSIGNALED(status) && WTERMSIG(status) == SIGABRT);
  }
#endif
  puts("C11 format writebacks, host conversions, bounds and truncation passed");
  return 0;
}
