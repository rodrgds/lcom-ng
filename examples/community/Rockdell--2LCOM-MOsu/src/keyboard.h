
#ifndef __KEYBOARD_H
#define __KEYBOARD_H

#include <minix/syslib.h>

/** @defgroup keyboard Keyboard
 *  @{
 *
 *  Keyboard related functions
 */

/**
 *  @brief Subscribes keyboard's interruptions.
 *  @return Returns an ID if successful and -1 otherwise.
 */
int keyboard_subscribe_int();

/**
 *  @brief Unsubscribes keyboard's interruptions.
 *  @return Returns 0 if successful and 1 otherwise.
 */
int keyboard_unsubscribe_int();

/**
 *  @brief Keyboard's interruption handler.
 *  @return Returns 0 if successful and 1 otherwise.
 */
int keyboard_int_handler();

/**
 *  @brief Prints the code.
 *
 *  When printing, it specifies if it's either a make or a break code.
 *  @param code Code to print. Gets the double code's key code.
 */
void printCode(unsigned long code);

/**
 *  @brief Prints a letter.
 *  @param code Letter to print.
 */
void printLetter(unsigned long code);

/**
 *  @brief Gets the last code received.
 *  @return Returns the last code processed.
 */
unsigned long getLastCode();

/**
 *  @brief Check if the last code processed was the double code (0xE0).
 *  @return Returns 1 if true and 0 otherwise.
 */
int getDoubleCode();

/** @} end of keyboard */

#endif
