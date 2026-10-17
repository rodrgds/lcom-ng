#ifndef _LCOM_MOUSE_H_
#define _LCOM_MOUSE_H_

#include <lcom/lcf.h>

int (mouse_subscribe_int)(uint8_t *bit_no);

int (mouse_unsubscribe_int)();

void (mouse_ih)();

void handle_mouse();

int (mouse_write)(uint8_t command);

int (mouse_disable_data_reporting)();

#endif
