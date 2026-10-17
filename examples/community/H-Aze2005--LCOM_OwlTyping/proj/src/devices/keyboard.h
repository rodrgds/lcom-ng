/**
 * @file keyboard.h
 * @brief Keyboard device interface and interrupt handling.
 *
 * Declares functions and variables for subscribing to, unsubscribing from,
 * and handling keyboard interrupts, as well as managing keyboard state.
 */

#ifndef _KEYBOARD_H_
#define _KEYBOARD_H_

#include <lcom/lcf.h>
#include <machine/int86.h>
#include "i8042.h"
#include "keyboard_layout.h"

/**
 * @brief Hook ID for keyboard interrupts.
 */
int keyboard_hook_id;

/**
 * @brief Buffer for storing keyboard scan codes (supports two-byte codes).
 */
uint8_t scancode[2];

/**
 * @brief Indicates if the last scan code was a make code (true) or break code (false).
 */
bool is_make;

/**
 * @brief Size of the current scan code (1 or 2 bytes).
 */
int size;

/**
 * @brief Indicates if the current scan code is two bytes.
 */
bool two_bytes;

/**
 * @brief Subscribes keyboard interrupts.
 * @param bit_no Address of variable to store the hook id bit number.
 * @return 0 on success, non-zero otherwise.
 */
int (subscribe_keyboard)(uint8_t *bit_no);

/**
 * @brief Unsubscribes keyboard interrupts.
 * @return 0 on success, non-zero otherwise.
 */
int (unsubscribe_keyboard)();

/**
 * @brief Keyboard interrupt handler.
 */
void (keyboard_ih)();

#endif
