#ifndef _LCOM_KEYBOARD_H_
#define _LCOM_KEYBOARD_H_

#include <minix/syslib.h>
#include <lcom/lcf.h>
#include "i8042.h" // Includes KBC related constants like IRQ_KBC, KBC_OUT_CMD, etc.

/**
 * @file keyboard.h
 * @brief Interface for programming the PS/2 Keyboard via the i8042 Keyboard Controller (KBC).
 *
 * This file declares functions for subscribing and unsubscribing keyboard interrupts,
 * handling keyboard interrupts (reading scancodes), and restoring the keyboard state.
 */

/** @defgroup keyboard Keyboard Interface
 * @{
 *
 * Functions for interacting with the PS/2 Keyboard.
 */



extern uint8_t scancode[2]; 

/**
 * @brief Subscribes to keyboard interrupts.
 *
 * Sets up the system to receive interrupts from the keyboard (via the KBC).
 * The IRQ line used by the keyboard (IRQ_KBC) will be enabled, and a hook ID
 * will be associated with it.
 *
 * @param bit_no_out Pointer to a variable where the bit number (hook_id) for keyboard interrupts will be stored.
 *                   This bit number should be used to check for keyboard interrupts in the main loop.
 * @return int 0 upon success, non-zero otherwise.
 */
int kbd_subscribe_int(uint8_t *bit_no_out);

/**
 * @brief Unsubscribes from keyboard interrupts.
 *
 * Disables keyboard interrupts and removes the associated hook ID.
 * It may also attempt to restore the KBC command byte to its state before subscription.
 *
 * @return int 0 upon success, non-zero otherwise.
 */
int kbd_unsubscribe_int();

/**
 * @brief Keyboard interrupt handler.
 *
 * This function should be called when a keyboard interrupt occurs.
 * It reads the scancode from the KBC's output buffer (KBC_OUT_CMD).
 * It handles potential communication errors and multi-byte scancodes,
 * storing the result in the global `scancode` array.
 */
void (kbc_ih)();

/**
 * @brief Restores the keyboard to a default state, typically by enabling interrupts.
 *
 * This function is often used after polling operations to ensure the keyboard
 * is back in interrupt-driven mode. It might involve writing to the KBC command byte
 * to re-enable keyboard interrupts.
 *
 * @return int 0 upon success, non-zero otherwise.
 */
int(kbd_restore)();

/**@}*/

#endif /* _LCOM_KEYBOARD_H_ */
