#ifndef PVZ_KEYBOARD_H
#define PVZ_KEYBOARD_H

#include "pvz_platform.h"

#include <lcom/i8042.h>

uint8_t get_scancode(void);
bool get_kbd_error(void);
int kbd_subscribe_int(uint8_t *bit_no);
int kbd_unsubscribe_int(void);
void kbc_ih(void);

#endif
