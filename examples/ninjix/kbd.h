#ifndef NINJIX_KBD_H
#define NINJIX_KBD_H

#include "ninjix_platform.h"

#ifndef ESC_BREAK
#define ESC_BREAK 0x81
#endif

#ifndef KBD_TWO_BYTE_PFX
#define KBD_TWO_BYTE_PFX 0xE0
#endif

#define KBD_BREAK_BIT BIT(7)

int kbd_process_byte(uint8_t byte);
int kbd_get_scancode(bool *make, uint8_t *size, uint8_t bytes[2]);

#endif
