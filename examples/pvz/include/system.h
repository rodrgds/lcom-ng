#ifndef SYSTEM_H
#define SYSTEM_H

#include "pvz_platform.h"

int init_systems(uint8_t *timer_bit, uint8_t *kbd_bit, uint8_t *mouse_bit);
int cleanup_systems(void);

#endif
