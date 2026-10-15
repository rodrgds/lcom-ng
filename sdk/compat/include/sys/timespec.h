#ifndef LCOM_NG_FORWARD_SYS_TIMESPEC
#define LCOM_NG_FORWARD_SYS_TIMESPEC
/* MSYS/Cygwin libc includes this header while defining its base POSIX types.
 * Prefer the host header so that path cannot recursively enter the SDK. */
#if __has_include_next(<sys/timespec.h>)
#include_next <sys/timespec.h>
#else
#include <lcom/compat.h>
#endif
#endif
