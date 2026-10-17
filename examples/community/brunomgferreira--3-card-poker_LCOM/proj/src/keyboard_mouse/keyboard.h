#ifndef __KEYBOARD_H
#define __KEYBOARD_H

#include <lcom/lcf.h>

/**
 * @defgroup keyboard Keyboard
 * @{
 *
 * Functions for interacting with the keyboard
 */

/**
 * @brief Subscribes to keyboard interrupts
 *
 * @param bit_no Pointer to a variable to store the bit mask for the interrupt
 * @return Return 0 upon success, non-zero otherwise
 */
int (kbd_subscribe_int)(uint8_t *bit_no);

/**
 * @brief Unsubscribes from keyboard interrupts
 *
 * @return Return 0 upon success, non-zero otherwise
 */
int (kbd_unsubscribe_int)();

/**
 * @brief Keyboard interrupt handler
 */
void (kbc_ih)();

/**
 * @brief Processes the received scancode
 */
void (process_scancode)();

/**
 * @brief Restores the keyboard to its default state
 *
 * @return Return 0 upon success, non-zero otherwise
 */
int (kbd_restore)();

/**
 * @brief Disables keyboard interrupts
 *
 * @return Return 0 upon success, non-zero otherwise
 */
int (kbd_disable_int)();

#endif /* __KEYBOARD_H */
