#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <stdint.h>

extern uint8_t scancode;

void (kbc_ih)();
int kbd_subscribe_int(uint8_t *bit_no);
int kbd_unsubscribe();

#endif /* KEYBOARD_H */
