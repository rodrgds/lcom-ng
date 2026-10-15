#ifndef __KEYBOARD_H
#define __KEYBOARD_H

#include <lcom/lcf.h>
#include <stdbool.h>
#include <stdint.h>

#include "i8042.h"

void (kbc_ih)();
int (keyboard_subscribe_int) (uint8_t* bit_n);
int (keyboard_unsubscribe_int) ();
int (check_status) (uint8_t st);


#endif
