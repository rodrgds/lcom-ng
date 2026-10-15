#ifndef __MOUSE_H
#define __MOUSE_H

#include <lcom/lcf.h>
#include <stdbool.h>
#include <stdint.h>

#include "i8042.h"

void (mouse_ih)();
int (mouse_subscribe_int)(uint8_t *bit_no);
int (mouse_unsubscribe_int)();

int(track_mouse_movement)();

#endif
