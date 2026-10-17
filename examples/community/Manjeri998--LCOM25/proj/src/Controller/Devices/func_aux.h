#ifndef _FUNC_AUX_H_
#define _FUNC_AUX_H_

#include <lcom/lcf.h>
#include <minix/sysutil.h>
#include <stdint.h>

int (aux_get_LSB)(uint16_t val, uint8_t *lsb);
int (aux_get_MSB)(uint16_t val, uint8_t *msb);
int (aux_sys_inb)(int port, uint8_t *value);

#endif 
