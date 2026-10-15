#ifndef LCOM_NG_MINIX_PATHS_H
#define LCOM_NG_MINIX_PATHS_H

#include <stdio.h>
#ifdef __cplusplus
extern "C" {
#endif
FILE *lcom_compat_fopen(const char *path, const char *mode);
int lcom_compat_remove(const char *path);
int lcom_compat_rename(const char *old_path, const char *new_path);
FILE *lcom_compat_freopen(const char *path, const char *mode, FILE *stream);
#ifdef __cplusplus
}
#endif

#ifndef LCOM_COMPAT_IMPLEMENTATION
#define fopen lcom_compat_fopen
#define freopen lcom_compat_freopen
#define remove lcom_compat_remove
#define rename lcom_compat_rename
#endif
#endif
