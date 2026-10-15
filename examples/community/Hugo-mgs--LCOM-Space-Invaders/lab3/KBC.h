#ifndef _LCOM_KBC_H_
#define _LCOM_KBC_H_

#include <minix/syslib.h>
#include "i8042.h"
#include <lcom/lcf.h>

int (KBC_get_status)(uint8_t *status);

int (KBC_read_output)(uint8_t port, uint8_t *out);

int (KBC_write_cmd_byte)(uint8_t port, uint8_t cmd);

int (KBC_reset)();

#endif
