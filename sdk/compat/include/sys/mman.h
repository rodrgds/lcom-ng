#pragma GCC system_header
#ifndef LCOM_COMPAT_SYS_MMAN_H
#define LCOM_COMPAT_SYS_MMAN_H
#include_next <sys/mman.h>
/* Minix 3.1.8 and 3.2.1 sys/mman.h expose minix/type.h directly. */
#include <minix/type.h>
#endif
