#ifndef _LCOM_KBC_KEY_H_
#define _LCOM_KBC_KEY_H_

#include <minix/sysutil.h>
#include <lcom/lcf.h>
#include "i8042.h"

int (KBC_read_status)(uint8_t* status);
int (KBC_read_output)(uint8_t port, uint8_t *output, uint8_t mouse);
int (KBC_write_command)(uint8_t port, uint8_t commandByte);

#endif
