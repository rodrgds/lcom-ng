#pragma once

#include <lcom/lcf.h>
#include "../kbc.h"

#define KBD_IRQ 1

#define ESC_BREAK 0x81
#define ARROW_UP 0x48
#define ARROW_DOWN 0x50
#define ENTER_MAKE 0X1C
#define B_LETTER 48

typedef struct {
  uint8_t buf[2];
  uint8_t size;
  bool make;
} scancode;

/**
 * Subscribe to keyboard interrupts
 * @param bit_no placeholder for hook id
 * @return 0 if successful, non-0 otherwise
*/
int kbd_subscribe_int(uint8_t *bit_no);

/**
 * @brief Unsubscribe to keyboard interrupts
 * @return 0 if successful, non-0 otherwise
*/
int kbd_unsubscribe_int();

/**
 * @brief Parse a keyboard scancode by updating it
 * with the most recent byte read, if any
 * 
 * @param sc scancode to be parsed
 * @return 0 if successful, 1 if incomplete, 2 if invalid
*/
int kbd_parse_scancode(scancode *sc);
