#pragma once

#include <stdbool.h>
#include <stdint.h>

#include "i8042.h"

int (keyboard_subscribe_int)(uint8_t *bit_no);

int (keyboard_unsubscribe_int)();

void (kbc_ih)(void);

int (keyboard_restore_interrupts)(void);

int (process_keyboard_esc)(void);
