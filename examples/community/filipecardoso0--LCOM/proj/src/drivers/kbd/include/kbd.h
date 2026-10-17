#ifndef _KBD_H_
#define _KBD_H_

#include <lcom/lcf.h>

#include "../../../utils/include/utils.h"
#include "kbd_macros.h"

extern int hook_id_kbd;
extern uint8_t scancode;

/**
 * @brief Parses KBD's scancode.
 * 
 * @return int 0 if succeded, 1 otherwise.
 */
int parse_scancode(bool*, size_t*, uint8_t*);

/**
 * @brief Handles the keyboard's interrupts.
 * 
 */
void (kbc_ih)(void);

/**
 * @brief Subscribes Keyboard's interrupts.
 * 
 */
int (kbd_subscribe)(uint8_t*);

/**
 * @brief Unsubscribes keyboard's interrupts.
 * 
 */
int (kbd_unsubscribe)();

#endif /* _KBD_H_ */ 
