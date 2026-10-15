/**
 * @file keyboard.h
 * @brief Interface for keyboard controller functions and interrupt handling.
 *
 * Provides functions to subscribe/unsubscribe keyboard interrupts, restore the keyboard state,
 * convert scancodes to ASCII characters, and the keyboard interrupt handler.
 */

#ifndef _LCOM_KEYBOARD_H_
#define _LCOM_KEYBOARD_H_

#include "KBC.h"
#include "i8042.h"
#include <lcom/lcf.h>
#include <minix/sysutil.h>

/**
 * @brief Keyboard interrupt handler. Reads scancode from the KBC output buffer.
 *
 * This function should be called on each keyboard interrupt to process the latest scancode.
 */
void(kbc_ih)();

/**
 * @brief Subscribes keyboard interrupts.
 *
 * @param bit_no Pointer to variable where the hook id will be stored (for use with sys_irqsetpolicy).
 * @return 0 on success, non-zero otherwise.
 */
int keyboard_subscribe_interrupts(uint8_t *bit_no);

/**
 * @brief Unsubscribes keyboard interrupts.
 *
 * @return 0 on success, non-zero otherwise.
 */
int keyboard_unsubscribe_interrupts();

/**
 * @brief Restores the keyboard to its default state.
 *
 * @return 0 on success, non-zero otherwise.
 */
int keyboard_restore();

/**
 * @brief Converts a keyboard scancode to its corresponding ASCII character, if possible.
 *
 * @param scancode The scancode to convert.
 * @return The corresponding character, or \0 if not a printable character.
 */
char scancode_to_char(uint8_t scancode);

#endif /* _LCOM_KEYBOARD_H_ */
