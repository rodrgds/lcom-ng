#ifndef __KBD_H
#define __KBD_H

#include "i8042.h"
#include "i8254.h"
#include "timer.h"

/** @defgroup KBD KBD
 * @{
 * Functions related to the KBD methods
 */

/**
 * Subscribes the keyboard peripherals interruptions
 */
int keyboard_subscribe_int();
/**
 * Unsubscribes the keyboard peripherals interruptions
 */
int keyboard_unsubscribe_int();

/**
 * Prints the MakeCodes and BreakCodes of the pressed keyboard keys
 * @param state scancodes from kbd
 * @return the scancodes
 */
unsigned long keyboard_long_print(unsigned long state);

/**
 * Reads Scancodes from kbd
 * @return the scancodes
 */
unsigned long keyboard_int_handler_c();

/** @} */

#endif
