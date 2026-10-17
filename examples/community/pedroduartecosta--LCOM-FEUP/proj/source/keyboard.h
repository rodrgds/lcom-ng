#ifndef __KEYBOARD_H
#define __KEYBOARD_H

#include "i8254.h"

/** @defgroup Keyboard Keyboard
 * @{
 * functions regarding the keyboard
 */


static int hook_kbd=NOTIFICATION_KBD;

/**
 * @brief subscribes keyboard interruption
 *
 * @return keyboard irq
 */

int kbd_subscribe();

/**
 * @brief unsubscribes keyboard interruption
 *
 * @return keyboard irq
 */

int kbd_unsubscribe();

/**
 * @brief gets the make code of the interruption
 *
 * @return keyboard make code
 */
int kbd_code_scan();

/**
 * @brief prints the make code
 *
 * @returns keyboard make code
 */
unsigned char kbd_code();

/**@}*/
#endif
