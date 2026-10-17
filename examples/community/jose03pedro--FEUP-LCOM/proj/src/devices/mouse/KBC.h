#ifndef _SRC_KBC_MOUSE_H_
#define _SRC_KBC_MOUSE_H_

#include "i8042.h"
#include <lcom/lcf.h>
#include <minix/sysutil.h>

int(read_KBC_status)(uint8_t *status);

int(read_KBC_output)(uint8_t port, uint8_t *output, uint8_t mouse);

int(write_KBC_command)(uint8_t port, uint8_t control);

#endif
