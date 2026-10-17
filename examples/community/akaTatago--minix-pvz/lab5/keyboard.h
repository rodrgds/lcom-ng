#ifndef __KBC_H
#define __KBC_H

#include <stdint.h>
#include <stdbool.h>

int (kbd_subscribe_int)(uint8_t *bit_no);
int (kbd_unsubscribe_int)();
void (kbc_ih)();

uint8_t (kbd_get_scancode)();
bool (kbd_has_error)();

int (kbc_restore_interrupts)();

#endif
