#define LCOM_COMPAT_IMPLEMENTATION
#include <lcom/printf.h>
#include <errno.h>
#include <inttypes.h>
#include <limits.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <wchar.h>

/* Bionic's vfprintf rejects %n itself, even without FORTIFY. Splitting only at
 * writebacks keeps floating point, wide characters and locale in the host libc.
 * The cursor below only advances argument types; it never formats their values.
 * See AOSP libc/stdio/vfprintf.cpp case 'n' and printf_common.h ADDSARG/ADDUARG. */
enum length { NORMAL, HH, H, L, LL, J, Z, T, LONG_DOUBLE };
struct conversion {
  const char *start, *end;
  enum length length;
  char code;
  bool width_star, precision_star, positional;
};

static bool digit(char c) { return c >= '0' && c <= '9'; }
static const char *digits(const char *p) { while (digit(*p)) ++p; return p; }

static bool conversion(const char **cursor, struct conversion *out) {
  const char *p = strchr(*cursor, '%');
  if (!p) return false;
  *out = (struct conversion){.start = p, .length = NORMAL};
  ++p;
  if (*p == '%') {
    out->code = '%'; out->end = p + 1; *cursor = out->end; return true;
  }
  const char *position = digits(p);
  if (position != p && *position == '$') { out->positional = true; p = position + 1; }
  while (*p && strchr("#0- +'", *p)) ++p;
  if (*p == '*') {
    out->width_star = true; position = digits(++p);
    if (*position == '$') { out->positional = true; p = position + 1; }
  } else p = digits(p);
  if (*p == '.') {
    ++p;
    if (*p == '*') {
      out->precision_star = true; position = digits(++p);
      if (*position == '$') { out->positional = true; p = position + 1; }
    } else p = digits(p);
  }
  switch (*p) {
    case 'h': out->length = *++p == 'h' ? (++p, HH) : H; break;
    case 'l': out->length = *++p == 'l' ? (++p, LL) : L; break;
    case 'j': out->length = J; ++p; break;
    case 'z': out->length = Z; ++p; break;
    case 't': out->length = T; ++p; break;
    case 'L': out->length = LONG_DOUBLE; ++p; break;
    case 'q': out->length = LL; ++p; break;
  }
  out->code = *p;
  out->end = *p ? p + 1 : p;
  *cursor = out->end;
  return true;
}

static bool valid_conversion(const struct conversion *spec) {
  switch (spec->code) {
    case 'd': case 'i': case 'o': case 'u': case 'x': case 'X': case 'b': case 'B': case 'n':
      return spec->length != LONG_DOUBLE;
    case 'a': case 'A': case 'e': case 'E': case 'f': case 'F': case 'g': case 'G':
      return spec->length == NORMAL || spec->length == L || spec->length == LONG_DOUBLE;
    case 'c': case 's': return spec->length == NORMAL || spec->length == L;
    case 'p': case '%': case 'm': return spec->length == NORMAL;
    default: return false;
  }
}

static void advance_argument(va_list *args, const struct conversion *spec) {
  if (spec->width_star) (void)va_arg(*args, int);
  if (spec->precision_star) (void)va_arg(*args, int);
  switch (spec->code) {
    case 'd': case 'i':
      switch (spec->length) {
        case L: (void)va_arg(*args, long); break;
        case LL: (void)va_arg(*args, long long); break;
        case J: (void)va_arg(*args, intmax_t); break;
        case Z: (void)va_arg(*args, ssize_t); break;
        case T: (void)va_arg(*args, ptrdiff_t); break;
        default: (void)va_arg(*args, int); break;
      }
      break;
    case 'o': case 'u': case 'x': case 'X': case 'b': case 'B':
      switch (spec->length) {
        case L: (void)va_arg(*args, unsigned long); break;
        case LL: (void)va_arg(*args, unsigned long long); break;
        case J: (void)va_arg(*args, uintmax_t); break;
        case Z: (void)va_arg(*args, size_t); break;
        case T: (void)va_arg(*args, uintptr_t); break;
        case HH: case H: (void)va_arg(*args, int); break;
        default: (void)va_arg(*args, unsigned int); break;
      }
      break;
    case 'a': case 'A': case 'e': case 'E': case 'f': case 'F': case 'g': case 'G':
      if (spec->length == LONG_DOUBLE) (void)va_arg(*args, long double);
      else (void)va_arg(*args, double);
      break;
    case 'c':
      if (spec->length == L) (void)va_arg(*args, wint_t);
      else (void)va_arg(*args, int);
      break;
    case 's':
      if (spec->length == L) (void)va_arg(*args, wchar_t *);
      else (void)va_arg(*args, char *);
      break;
    case 'p': (void)va_arg(*args, void *); break;
  }
}

static void write_count(va_list *args, enum length length, int count) {
  switch (length) {
    case HH: *va_arg(*args, signed char *) = count; break;
    case H: *va_arg(*args, short *) = count; break;
    case L: *va_arg(*args, long *) = count; break;
    case LL: *va_arg(*args, long long *) = count; break;
    case J: *va_arg(*args, intmax_t *) = count; break;
    case Z: *va_arg(*args, ssize_t *) = count; break;
    case T: *va_arg(*args, ptrdiff_t *) = count; break;
    default: *va_arg(*args, int *) = count; break;
  }
}

struct output { char *buffer; size_t size, written; FILE *stream; int count, original_errno; };

static int emit(struct output *output, const char *format, va_list args) {
  va_list copy;
  va_copy(copy, args);
  errno = output->original_errno;
  int result;
  if (output->stream) result = vfprintf(output->stream, format, copy);
  else {
    size_t available = output->size - output->written;
    result = vsnprintf(available ? output->buffer + output->written : NULL, available, format, copy);
    if (result >= 0 && available) {
      size_t written = (size_t)result < available ? (size_t)result : available - 1;
      output->written += written;
    }
  }
  va_end(copy);
  if (result < 0) return -1;
  if (result > INT_MAX - output->count) { errno = EOVERFLOW; return -1; }
  output->count += result;
  return 0;
}

static int format(struct output *output, const char *text, va_list args) {
  if (!text) { errno = EINVAL; return -1; }
  const char *cursor = text;
  struct conversion spec;
  bool writeback = false, positional = false, valid = true;
  while (conversion(&cursor, &spec)) {
    writeback |= spec.code == 'n'; positional |= spec.positional;
    valid &= valid_conversion(&spec);
    if (!spec.code) break;
  }
  if (!writeback) return emit(output, text, args) ? -1 : output->count;
  if (positional || !valid) { errno = positional ? ENOTSUP : EINVAL; return -1; }
  char *part = malloc(strlen(text) + 1);
  if (!part) { errno = ENOMEM; return -1; }
  va_list remaining, segment_args;
  va_copy(remaining, args); va_copy(segment_args, args);
  const char *segment = text;
  cursor = text;
  int result = -1;
  while (conversion(&cursor, &spec)) {
    if (spec.code != 'n') { advance_argument(&remaining, &spec); continue; }
    size_t length = (size_t)(spec.start - segment);
    memcpy(part, segment, length); part[length] = 0;
    if (emit(output, part, segment_args)) goto done;
    if (spec.width_star) (void)va_arg(remaining, int);
    if (spec.precision_star) (void)va_arg(remaining, int);
    write_count(&remaining, spec.length, output->count);
    va_end(segment_args); va_copy(segment_args, remaining);
    segment = spec.end;
  }
  if (!emit(output, segment, segment_args)) result = output->count;
done:
  va_end(segment_args); va_end(remaining);
  int saved_errno = result < 0 ? errno : output->original_errno;
  free(part); errno = saved_errno;
  return result;
}

int lcom_compat_vsnprintf(char *buffer, size_t size, const char *text, va_list args) {
  if (size && !buffer) { errno = EINVAL; return -1; }
  struct output output = {.buffer = buffer, .size = size, .original_errno = errno};
  if (size) buffer[0] = 0;
  return format(&output, text, args);
}
int lcom_compat_vfprintf(FILE *stream, const char *text, va_list args) {
  struct output output = {.stream = stream, .original_errno = errno};
  flockfile(stream);
  int result = format(&output, text, args);
  funlockfile(stream);
  return result;
}
int lcom_compat_vsprintf(char *buffer, const char *text, va_list args) {
  return lcom_compat_vsprintf_checked(buffer, SIZE_MAX, text, args);
}
int lcom_compat_vprintf(const char *text, va_list args) { return lcom_compat_vfprintf(stdout, text, args); }

static void bounds_failure(void) {
  fputs("lcom-ng: formatted output exceeds destination object bounds\n", stderr);
  abort();
}
int lcom_compat_vsnprintf_checked(char *buffer, size_t size, size_t capacity, const char *text, va_list args) {
  if (capacity != SIZE_MAX && size > capacity) bounds_failure();
  return lcom_compat_vsnprintf(buffer, size, text, args);
}
int lcom_compat_vsprintf_checked(char *buffer, size_t capacity, const char *text, va_list args) {
  size_t size = capacity == SIZE_MAX ? (size_t)SSIZE_MAX : capacity;
  int result = lcom_compat_vsnprintf(buffer, size, text, args);
  if (result >= 0 && capacity != SIZE_MAX && (size_t)result >= capacity) bounds_failure();
  return result;
}

int lcom_compat_snprintf(char *buffer, size_t size, const char *text, ...) {
  va_list args; va_start(args, text);
  int result = lcom_compat_vsnprintf(buffer, size, text, args);
  va_end(args); return result;
}
int lcom_compat_sprintf(char *buffer, const char *text, ...) {
  va_list args; va_start(args, text);
  int result = lcom_compat_vsprintf(buffer, text, args);
  va_end(args); return result;
}
int lcom_compat_fprintf(FILE *stream, const char *text, ...) {
  va_list args; va_start(args, text);
  int result = lcom_compat_vfprintf(stream, text, args);
  va_end(args); return result;
}
int lcom_compat_printf(const char *text, ...) {
  va_list args; va_start(args, text);
  int result = lcom_compat_vprintf(text, args);
  va_end(args); return result;
}
int lcom_compat_snprintf_checked(char *buffer, size_t size, size_t capacity, const char *text, ...) {
  va_list args; va_start(args, text);
  int result = lcom_compat_vsnprintf_checked(buffer, size, capacity, text, args);
  va_end(args); return result;
}
int lcom_compat_sprintf_checked(char *buffer, size_t capacity, const char *text, ...) {
  va_list args; va_start(args, text);
  int result = lcom_compat_vsprintf_checked(buffer, capacity, text, args);
  va_end(args); return result;
}
