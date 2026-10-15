#ifndef _KBD_H_
#define _KBD_H_

#include <minix/syslib.h>
#include <lcom/lcf.h>
#include "i8042.h"
#include "KBC.h"

/**
 * Subscribes to keyboard interrupts.
 * 
 * @param bit_no Pointer to a variable where the bit number to be set in the mask will be stored.
 * @return 0 on success, non-zero otherwise.
 */
int (kbd_subscribe_int)(uint8_t *bit_no);

/**
 * Unsubscribes from keyboard interrupts.
 *
 * @return 0 on success, non-zero otherwise.
 */
int (kbd_unsubscribe_int)();

/**
 * Keyboard interrupt handler.
 */
void (kbc_ih)(void);

#endif


