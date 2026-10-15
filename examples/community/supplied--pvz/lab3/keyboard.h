#ifndef _LCOM_KEYBOARD_H_
#define _LCOM_KEYBOARD_H_

#include <stdbool.h>
#include <stdint.h>

uint8_t get_scancode();
bool get_kbd_error();

int (kbd_subscribe_int)(uint8_t *bit_no);
int (kbd_unsubscribe_int)();
void (kbc_ih)();
int (kbd_poll)();
int (kbc_restore_interrupts)();

#endif /* _LCOM_KEYBOARD_H_ */

