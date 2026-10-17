#ifndef __UTILS_H
#define __UTILS_H

#include <lcom/lcf.h>

// Utils
int(util_get_LSB)(uint16_t val, uint8_t *lsb);
int(util_get_MSB)(uint16_t val, uint8_t *msb);

// Wrapper Function
int util_sys_inb(int port, uint8_t* read);

#endif /* __UTILS_H */
