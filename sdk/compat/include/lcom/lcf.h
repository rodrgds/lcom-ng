#ifndef LCOM_NG_FORWARD_LCOM_LCF
#define LCOM_NG_FORWARD_LCOM_LCF
#ifndef UNUSED
#define UNUSED(argument) argument __attribute__((unused))
#endif
#include <stdbool.h>
#include <time.h>
#include <lcom/compat.h>
#include <lcom/read_xpm.h>
#include <lcom/timer.h>
#include <lcom/lab2.h>
#include <lcom/lab3.h>
#include <lcom/lab4.h>
#include <lcom/lab5.h>
/* Original LCF substitutes calls only in instrumented/optimized sources.
 * Function-like expansion preserves student objects and libc function pointers. */
int rand_(void);
#if defined(__LCOM_OPTIMIZED__) || defined(__LCOM__)
#define rand() rand_()
#endif
int proj_demo(uint16_t mode, bool minix3_logo, bool grayscale, uint8_t delay);
#endif
