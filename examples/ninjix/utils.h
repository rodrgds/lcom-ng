#ifndef NINJIX_UTILS_H
#define NINJIX_UTILS_H

#include "ninjix_platform.h"

#include <stdint.h>

extern int sys_inb_counter;
extern int sys_outb_counter;

int util_sys_inb(int port, uint8_t *value);
int util_sys_outb(int port, uint8_t value);
void util_force_tty_redraw(void);
int16_t util_clamp_i16(int32_t value, int16_t min_value, int16_t max_value);
int32_t util_clamp_i32(int32_t value, int32_t min_value, int32_t max_value);

#endif
