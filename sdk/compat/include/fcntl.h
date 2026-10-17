#pragma GCC system_header
#ifndef LCOM_COMPAT_FCNTL_H
#define LCOM_COMPAT_FCNTL_H
#include_next <fcntl.h>

/* Preserve POSIX descriptor I/O through the same declared project aliases as
 * stdio, only after the source imports open's owning header. */
#ifdef __cplusplus
extern "C" {
#endif
int lcom_compat_open(const char *path, int flags, ...);
#ifdef __cplusplus
}
#endif
#ifndef LCOM_COMPAT_IMPLEMENTATION
#define open lcom_compat_open
#endif
#endif
