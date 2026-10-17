/**
 * @file keyboard.h
 * @author Bruno Pinto and Vitor Magalhaes
 * @brief Contains keyboard functions
 */

#ifndef _KEYBOARD_H_
#define _KEYBOARD_H_

/** @defgroup keyboard keyboard
 * @{
 *
 * Functions related to keyboard
 */

/**
 * @brief Subscribes and enables Keyboard interruptss

 *
 * @return Returns bit order in interrupt mask; negative value on failure
 */
int kbd_subscribe(void);

/**
 * @brief Unsubscribes Keyboard interrupts
 *
 * @return Return 0 upon success and non-zero otherwise
 */
int kbd_unsubscribe(void);

/**
 * @brief Reads scancodes via KBD interrupts
 *
 * @return Returns scancode read
 */
unsigned long kbd_scan();

/** @} end of keyboard */

#endif
