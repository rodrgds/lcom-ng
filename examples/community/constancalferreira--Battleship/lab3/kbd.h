#ifndef _LCOM_KBD_H_
#define _LCOM_KBD_H_

#include <stdint.h>

void (kbc_ih)();
int (kbd_subscribe_int)(uint8_t *bit_no);
int (kbd_unsubscribe_int)();

//polling
int (kbd_restore)();

//process multi-byte scancodes
void (process_scancode)(uint8_t scancode);

#endif
