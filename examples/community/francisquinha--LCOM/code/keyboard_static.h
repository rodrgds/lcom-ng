#ifndef __KEYBOARD_STATIC_H
#define __KEYBOARD_STATIC_H

/** @file keyboard_static.h
 * @brief file containing declarations of the functions that deal with the keyboard
 */

/** @defgroup keyboard_static keyboard_static
 * @{
 *
 * @brief static unctions for using the i8042 keyboard
 */

/**
 * @brief obtains the keyboard scan code
 * @return the latest keyboard scan code
 */
static long int kbd_int_processor_c();

/** @} end of keyboard_static */

#endif /* __KEYBOARD_STATIC_H */
