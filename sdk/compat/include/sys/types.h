#pragma GCC system_header
#ifndef LCOM_COMPAT_SYS_TYPES_H
#define LCOM_COMPAT_SYS_TYPES_H
#include_next <sys/types.h>
#include <stdint.h>
/* Minix machine/int_types.h also exposes the internal unsigned-byte name.
 * The native platform headers supply it; Emscripten musl does not. */
#ifdef __EMSCRIPTEN__
typedef uint8_t __uint8_t;
#endif
/* Minix's NetBSD sys/types.h exposes these fixed-width BSD spellings. */
typedef uint8_t u_int8_t;
typedef uint16_t u_int16_t;
typedef uint32_t u_int32_t;
typedef uint64_t u_int64_t;
#endif
