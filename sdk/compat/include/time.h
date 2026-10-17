#pragma GCC system_header
#ifndef LCOM_COMPAT_TIME_H
#define LCOM_COMPAT_TIME_H

#ifdef LCOM_COMPAT_LEGACY_TIME_ALIAS
#undef time
#undef LCOM_COMPAT_LEGACY_TIME_ALIAS
#endif
#include_next <time.h>

#endif
