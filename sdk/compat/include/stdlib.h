#pragma GCC system_header
#ifndef LCOM_COMPAT_STDLIB_H
#define LCOM_COMPAT_STDLIB_H

/* Older Darwin SDKs import the process-wait API through stdlib.h. Minix's
 * stdlib does not reserve that name. Keep the host extension private while
 * retaining the explicit sys/wait.h interface. */
#ifdef __APPLE__
#pragma push_macro("wait")
#undef wait
#define wait lcom_compat_darwin_wait
#define LCOM_COMPAT_STDLIB_IMPORT
#include_next <stdlib.h>
#undef LCOM_COMPAT_STDLIB_IMPORT
#pragma pop_macro("wait")
#else
#include_next <stdlib.h>
#endif

/* The Minix/NetBSD stdlib includes sys/types for its BSD interfaces. */
#include <sys/types.h>

#ifdef __cplusplus
extern "C"
#endif
int reallocarr(void *pointer, size_t count, size_t size);

#endif
