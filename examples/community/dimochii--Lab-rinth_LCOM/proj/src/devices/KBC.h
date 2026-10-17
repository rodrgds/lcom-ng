#ifndef _KBC_H_
#define _KBC_H_

#include <minix/sysutil.h>
#include <lcom/lcf.h>
#include "i8042.h"

int (write_kbc)(uint8_t port, uint8_t command);
int (read_kbc)(uint8_t port, uint8_t *byte, uint8_t mouse_aux);

#endif
