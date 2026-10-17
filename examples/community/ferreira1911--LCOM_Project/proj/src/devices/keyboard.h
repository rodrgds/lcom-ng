#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <lcom/lcf.h>

#define KBD_IRQ 1 /**< @brief Keyboard IRQ line */

/* Scancode constants */

#define ESC_BREAKCODE 0x81 /**< @brief ESC key breakcode */
#define BREAKCODE_BIT 0x80    /**< @brief Breakcode identifier */

extern uint8_t scancode;
extern bool scancode_ready;

int (kbd_unsubscribe_int)();
int (kbd_subscribe_int)(uint8_t *bit_no);
void (kbc_ih)(void);

#endif

