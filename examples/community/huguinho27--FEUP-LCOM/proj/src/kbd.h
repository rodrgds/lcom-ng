#ifndef __KBD_H
#define __KBD_H

/** @defgroup kbd kbd
 * @{
 * Functions for manipulating keyboard inputs
 */

/**
 * @brief Subscribes interruptions of keyboard
 * @return 1 on error, 0 on success
 */
int kbd_subscribe_int();

/**
 * @brief Unsubscribes interruptions of keyboard
 * @return 1 on error, 0 on success
 */
int kbd_unsubscribe_int();

/**
 * @brief Reads information from keyboard controller
 * @return Scancode of the key pressed or released
 */
int kbd_handler();

/**
 * @brief Reads information from keyboard controller
 * @return 1 on error, 0 on success
 */
int readInfoFromKBD(unsigned long *data);

/**
 * @brief Writes information to the keyboard controller
 * @return 1 on error, 0 on success
 */
int writeInfoToKBD(unsigned long kbd_port);

static int twoBytes = 0;
static unsigned char NUM_LOCK = 0;
static unsigned char CAPS_LOCK = 0;
static unsigned char SCROLL_LOCK = 0;

#endif /**@}*/
