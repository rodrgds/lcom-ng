#ifndef LCOM_NG_COMPAT_READ_XPM_H
#define LCOM_NG_COMPAT_READ_XPM_H
#include <lcom/libxpm.h>
/* Pre-LCF indexed pixmaps return a caller-owned byte per pixel. */
char *read_xpm(char *map[], int *width, int *height);
#endif
