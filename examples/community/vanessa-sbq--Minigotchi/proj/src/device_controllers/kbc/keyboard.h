#ifndef __KEYBOARD_H
#define __KEYBOARD_H

#include "../utils.h"
#include "kbc.h"
#include "i8042.h"

#include <lcom/lcf.h>

int subscribe_interrupts_kbd(uint8_t* bit_no);
int unsubscribe_interrupts_kbd();
//void (kbc_ih)();
uint8_t getScanCode();

#endif /* __KBC_H */
