#pragma once

#include <lcom/lcf.h>
#include "i8042.h"

int (kbc_read_output)(uint8_t* scancode, uint8_t aux_mouse_bit);

int (kbc_write_command)(uint8_t port, uint8_t commandByte);
