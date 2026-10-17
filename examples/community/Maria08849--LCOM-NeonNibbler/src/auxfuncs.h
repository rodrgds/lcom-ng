#ifndef _LCOM_AUXFUNCS_H_
#define _LCOM_AUXFUNCS_H_

#include "kbc.h"
#include <lcom/lcf.h>
#include <minix/sysutil.h>
#include <stdint.h>

int(aux_get_LSB)(uint16_t val, uint8_t *lsb);
int(aux_get_MSB)(uint16_t val, uint8_t *msb);
int(aux_sys_inb)(int port, uint8_t *value);
int(aux_sys_inb_16)(int port, uint16_t *value);

#endif /* _LCOM_AUXFUNCS_H_ */
