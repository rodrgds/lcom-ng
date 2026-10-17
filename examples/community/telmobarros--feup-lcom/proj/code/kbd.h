/**
 * This module contains all functions to manage keyboard
 * Jorge Vale: 50%
 * Telmo Barros: 50%
 * Relative Weight:  4%
 */
#ifndef __KBD_H
#define __KBD_H

/** @defgroup Keyboard Keyboard
 * @{
 * Functions to manage keyboard
 */

/**
 * @brief Subscribes the keyboard interrupts
 *
 * @return Return keyboard ID to verify the interrupts and -1 upon failure
 */
int kbd_subscribe_int();

/**
 * @brief Unsubscribes the keyboard interrupts
 *
 * @return Return 0 upon success and 1 upon failure
 */
int kbd_unsubscribe_int();

/**
 * @brief Reads from the output buffer of the keyboard
 *
 * @return Return data of buffer and -1 upon failure
 */
int kbd_output();

/**@}*/

#endif
