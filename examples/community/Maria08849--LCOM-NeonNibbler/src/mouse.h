#ifndef _LCOM_MOUSE_H_
#define _LCOM_MOUSE_H_

#include "kbc.h"
#include <lcom/lcf.h>
#include <minix/sysutil.h>

int(mouse_subscribe_interrupts)(uint8_t *bit_no);

int(mouse_unsubscribe_interrupts)();

void(mouse_ih)();

int(get_mouse_data)();

int(process_mouse_data)(uint8_t data, uint32_t *cnt, int *counter, struct packet *pack);

int(process_mouse_data_t)(uint8_t data, int *counter, struct packet *pack);

#endif
