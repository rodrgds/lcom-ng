#ifndef __PROJ_KBD_H
#define __PROJ_KBD_H

#include "controller/kbc/kbc.h"

/** @defgroup kbd kbd
 * @{
 *
 * Functions for interacting with the Keyboard.
 */

/**
 * @brief Getter for the scancode variable
 *
 * @return The current value of the scancode
 */
uint8_t(get_scancode)(void);

/**
 * @brief Setter for the scancode variable
 *
 * @param value The value to set the scancode to
 */
void(set_scancode)(uint8_t value);

/**
 * @brief Subscribes keyboard interrupts
 *
 * @param bit_no Address of memory position to be filled with the hook_id
 * @return Return 0 upon success and non-zero otherwise
 */
int(kbd_subscribe_int)(uint8_t *bit_no);

/**
 * @brief Unsubscribes keyboard interrupts
 *
 * @return Return 0 upon success and non-zero otherwise
 */
int(kbd_unsubscribe_int)(void);

/**
 * @brief Enables keyboard interrupts
 *
 * @return Return 0 upon success and non-zero otherwise
 */
int(kbd_enable_int)(void);

/**
 * @brief Keyboard interrupt handler
 *
 * Reads the scancode from the KBC output buffer
 */
void(kbd_ih)(void);

/**@}*/

#endif /* __PROJ_KBD_H */
