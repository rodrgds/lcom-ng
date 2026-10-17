#ifndef LCOM_NG_MINIX_UNISTD_H
#define LCOM_NG_MINIX_UNISTD_H
#include_next <unistd.h>

/* File-existence and permission probes use the same declared project paths
 * as the subsequent stdio or descriptor open. Retain libc's mode semantics. */
#ifdef __cplusplus
extern "C" {
#endif
int lcom_compat_access(const char *path, int mode);
#ifdef __cplusplus
}
#endif
#ifndef LCOM_COMPAT_IMPLEMENTATION
#define access lcom_compat_access
#endif
#endif
