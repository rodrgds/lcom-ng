#ifndef _LCOM_KBC_H_
#define _LCOM_KBC_H_

#include <lcom/lcf.h>
#include "KBC.h"

int(keyboard_subscribe_int)(uint8_t *bit_no);
int(keyboard_unsubscribe_int)();
int(keyboard_enable_interrupts)();
void (kbc_ih)();
int (await_ESC_break)();

#endif
