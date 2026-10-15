#ifndef LCOM_NG_FORWARD_MALLOC_H
#define LCOM_NG_FORWARD_MALLOC_H
/* Keep libc's own declarations on Android/Linux. macOS exposes the standard
 * allocation functions through stdlib.h instead of a top-level malloc.h. */
#if __has_include_next(<malloc.h>)
#include_next <malloc.h>
#else
#include <stdlib.h>
#endif
#endif
