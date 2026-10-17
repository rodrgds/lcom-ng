#ifndef _LCOM_MOUSE_H_
#define _LCOM_MOUSE_H_

#include <minix/sysutil.h>
#include <lcom/lcf.h>
#include "KBC.h"

int (write_mouse) (uint8_t command);

int (mouse_subscribe_int)(uint8_t* irq_set);
int (mouse_unsubscribe_int)();

void (mouse_ih)();
void (store_byte_mouse)();
void (store_bytes_packet)();

#endif
