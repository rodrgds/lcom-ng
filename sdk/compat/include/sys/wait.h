#pragma GCC system_header
#include_next <sys/wait.h>

/* Repeated explicit imports must restore wait after Darwin's guarded system
 * header was first reached privately from stdlib.h. */
#if defined(__APPLE__) && !defined(LCOM_COMPAT_STDLIB_IMPORT)
#ifdef __cplusplus
extern "C" {
#endif
pid_t wait(int *) __DARWIN_ALIAS_C(wait);
#ifdef __cplusplus
}
#endif
#endif
