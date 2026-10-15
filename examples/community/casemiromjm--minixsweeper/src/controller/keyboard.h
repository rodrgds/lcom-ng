/**
 * @file keyboard.h
 * @brief Header file for keyboard interrupt handling and reactivation.
 */

#ifndef _KEYBOARD_H_
#define _KEYBOARD_H_

#include <lcom/lcf.h>
#include <minix/sysutil.h>
#include <stdint.h>

#include "kbc.h"

/**
 * @brief Subscribes keyboard interrupts.
 * 
 * Configures the system to receive interrupts from the keyboard.
 * 
 * @param bit_no Pointer to a variable where the bit number will be stored.
 * @return 0 on success, 1 on failure.
 */
int (kbd_subscribe_int)(uint8_t *bit_no);

/**
 * @brief Unsubscribes keyboard interrupts.
 * 
 * Disables the keyboard interrupt policy previously set.
 * 
 * @return 0 on success, 1 on failure.
 */
int (kbd_unsubscribe_int)();

/**
 * @brief Keyboard interrupt handler.
 * 
 * Reads the scancode from the output buffer if available.
 */
void (kbc_ih)();

/**
 * @brief Re-enables keyboard interrupts by modifying the KBC command byte.
 * 
 * Reads the current command byte, modifies it to re-enable interrupts, and writes it back.
 * 
 * @return 0 on success, 1 on failure.
 */
int (kbd_reenable)();

#endif /* _KEYBOARD_H_ */

