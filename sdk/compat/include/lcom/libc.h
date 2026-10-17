#ifndef LCOM_NG_MINIX_LIBC_H
#define LCOM_NG_MINIX_LIBC_H

/* Preserve the legacy importer's libc declarations and 64-bit host adapters
 * without importing Minix device names into unrelated student source files.
 * Device types, constants and APIs belong to their explicit owning headers. */
#include <stdio.h>
#include <lcom/paths.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <errno.h>
#include <limits.h>
#include <unistd.h>
#include <sys/types.h>
#include <lcom/printf.h>

/* Preserve the pre-LCF importer's typed conversion API for legacy pure C
 * units. Modern sources obtain it only from an explicit Minix minlib owner. */
#ifdef LCOM_COMPAT_LEGACY
char *itoa(int value);
#endif

/* The Minix umbrella exposes time helpers, but does not reserve the name
 * clock used by older student graphics modules. Programs needing struct tm
 * members should include <time.h>, as with ordinary host C code. */
#ifndef LCOM_COMPAT_LEGACY
struct tm;
time_t time(time_t *result);
char *ctime(const time_t *timer);
struct tm *localtime(const time_t *timer);
struct tm *gmtime(const time_t *timer);
double difftime(time_t end, time_t beginning);
time_t mktime(struct tm *date);
size_t strftime(char *buffer, size_t size, const char *format, const struct tm *date);
#endif

unsigned int lcom_compat_sleep(unsigned int seconds);
int lcom_compat_usleep(unsigned int micros);
#ifndef LCOM_COMPAT_IMPLEMENTATION
#define sleep lcom_compat_sleep
#define usleep lcom_compat_usleep
#endif
#ifdef LCOM_COMPAT_LEGACY
/* The Minix 3.3 driver umbrella exposes time() transitively, while earlier
 * projects can own objects named time or clock. Preserve those object names
 * without calling libc through an implicit-int declaration. An explicit
 * <time.h> imports the ordinary function and function-address contract. */
time_t lcom_compat_time(time_t *result);
#if !defined(LCOM_COMPAT_IMPLEMENTATION) && !defined(LCOM_COMPAT_TIME_H)
#define LCOM_COMPAT_LEGACY_TIME_ALIAS
#define time(result) lcom_compat_time(result)
#endif

#endif
#endif
