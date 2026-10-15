#ifndef _KEYBOARD_H_
#define _KEYBOARD_H_

/**
 * @file keyboard.h
 * @brief Keyboard interface functions.
 */

#include <lcom/lcf.h>
#include "../KBC/i8042.h"
#include "../KBC/KBC.h"

/** @defgroup keyboard keyboard
 * @{
 */

/**
 * @brief Subscribes to keyboard interrupts.
 *
 * @param keyboard_irq Pointer to a variable where the keyboard IRQ bit mask will be stored.
 * @return 0 on success, non-zero value on failure.
 */
int (keyboard_subscribe_int)(uint32_t *keyboard_irq);

/**
 * @brief Unsubscribes from keyboard interrupts.
 *
 * @return 0 on success, non-zero value on failure.
 */
int (keyboard_unsubscribe_int)();

/**
 * @brief Reads the scancode from the keyboard.
 */
void (read_scancode)();

/**
 * @brief Enables keyboard interrupts.
 *
 * @return 0 on success, non-zero value on failure.
 */
int (enable_keyboard_int)();

/** @} */ // end of Keyboard group

#endif
