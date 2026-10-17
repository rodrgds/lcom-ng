/* Compiler stdarg headers permit repeated partial includes. Forward every
 * include, while declaring the Minix 3.3 sys/ansi.h variadic-list alias once. */
#include_next <stdarg.h>
#ifndef LCOM_NG_MINIX_VA_LIST_DECLARED
#define LCOM_NG_MINIX_VA_LIST_DECLARED
/* Minix sys/ansi.h uses the compiler-owned ABI, never an assumed pointer. */
typedef __builtin_va_list __va_list;
#endif
