#ifndef LCOM_NG_COMPAT_PRINTF_H
#define LCOM_NG_COMPAT_PRINTF_H
#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif
int lcom_compat_vsnprintf(char *, size_t, const char *, va_list);
int lcom_compat_snprintf(char *, size_t, const char *, ...) __attribute__((format(printf, 3, 4)));
int lcom_compat_vsprintf(char *, const char *, va_list);
int lcom_compat_sprintf(char *, const char *, ...) __attribute__((format(printf, 2, 3)));
int lcom_compat_vfprintf(FILE *, const char *, va_list);
int lcom_compat_fprintf(FILE *, const char *, ...) __attribute__((format(printf, 2, 3)));
int lcom_compat_vprintf(const char *, va_list);
int lcom_compat_printf(const char *, ...) __attribute__((format(printf, 1, 2)));
int lcom_compat_vsnprintf_checked(char *, size_t, size_t, const char *, va_list);
int lcom_compat_snprintf_checked(char *, size_t, size_t, const char *, ...) __attribute__((format(printf, 4, 5)));
int lcom_compat_vsprintf_checked(char *, size_t, const char *, va_list);
int lcom_compat_sprintf_checked(char *, size_t, const char *, ...) __attribute__((format(printf, 3, 4)));
#ifdef __cplusplus
}
#endif

/* Bionic rejects the C11 %n conversion. Keep host formatting for every other
 * conversion and preserve compiler-known destination bounds at ordinary calls.
 * Positional formats combined with %n return -1 with errno=ENOTSUP.
 * The second macro layer also leaves address-taking bound to a real function. */
#if defined(__ANDROID__) && !defined(LCOM_COMPAT_IMPLEMENTATION)
#define sprintf lcom_compat_sprintf
#define vsprintf lcom_compat_vsprintf
#define snprintf lcom_compat_snprintf
#define vsnprintf lcom_compat_vsnprintf
#define printf lcom_compat_printf
#define vprintf lcom_compat_vprintf
#define fprintf lcom_compat_fprintf
#define vfprintf lcom_compat_vfprintf
#define lcom_compat_sprintf(dst, ...) lcom_compat_sprintf_checked((dst), __builtin_object_size((dst), 0), __VA_ARGS__)
#define lcom_compat_vsprintf(dst, fmt, args) lcom_compat_vsprintf_checked((dst), __builtin_object_size((dst), 0), (fmt), (args))
#define lcom_compat_snprintf(dst, size, ...) lcom_compat_snprintf_checked((dst), (size), __builtin_object_size((dst), 0), __VA_ARGS__)
#define lcom_compat_vsnprintf(dst, size, fmt, args) lcom_compat_vsnprintf_checked((dst), (size), __builtin_object_size((dst), 0), (fmt), (args))
#endif
#endif
