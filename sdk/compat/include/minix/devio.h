#ifndef LCOM_NG_COMPAT_MINIX_DEVIO_H
#define LCOM_NG_COMPAT_MINIX_DEVIO_H
#include <lcom/compat.h>

/* Minix SYS_VDEVIO pairs retain the guest's fixed-width port/value ABI. */
typedef struct { uint16_t port; uint8_t value; } pvb_pair_t;
typedef struct { uint16_t port; uint16_t value; } pvw_pair_t;
typedef struct { uint16_t port; uint32_t value; } pvl_pair_t;

#define pv_set(pair, port_value, data_value) do { \
    uint32_t lcom_pv_port = (port_value), lcom_pv_value = (data_value); \
    (pair).port = lcom_pv_port; \
    (pair).value = lcom_pv_value; \
    if ((pair).port != lcom_pv_port || (pair).value != lcom_pv_value) \
        panic("pv_set: port or value does not fit the device I/O pair"); \
} while (0)
#endif
