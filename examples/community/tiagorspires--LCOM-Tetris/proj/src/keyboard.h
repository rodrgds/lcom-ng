/**
 * @file keyboard.h
 * @brief Keyboard interface functions and definitions.
 */

#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <lcom/lcf.h>
#include "i8042.h"
#include <stdint.h>

/**
 * @brief Subscribes to keyboard interrupts.
 * 
 * @param bit_no Address of memory to be initialized with the bit number to be set in the mask returned upon an interrupt.
 * @return 0 upon success, 1 otherwise.
 */
int (keyboard_subscribe)(uint8_t *bit_no);

/**
 * @brief Unsubscribes from keyboard interrupts.
 * 
 * @return 0 upon success, 1 otherwise.
 */
int (keyboard_unsubscribe) ();

/**
 * @brief Keyboard interrupt handler.
 * Reads the scancode from the output buffer.
 */
void (kbc_ih_keyboard)();

/**
 * @brief Waits for the ESC key to be pressed.
 * 
 * @return 0 upon success, 1 otherwise.
 */
int (escape_key) ();

#endif // KEYBOARD_H
