#ifndef KEYBOARD_H
#define KEYBOARD_H

/** @defgroup kbc keyboard
 * @{
 *
 * Functions related to the keyboard driver.
 */

#include <lcom/lcf.h>
#include <stdint.h>

#include <minix/syslib.h>

#include "i8042.h"
#include "event.h"

/**
 * @brief Handle keyboard interrupts. Reads scan codes and sets read_ok when the reading is successful.
 * 
 */
void (kbc_ih)();

/**
 * @brief Subscribe keyboard interrupts and disable Minix default handler.
 * @param bit_no Receives the bit that will be set for keyboard interrupts.
 * @return Returns 0 upon success, and 1 otherwise.
 */
int (keyboard_subscribe_int)(uint8_t *bit_no);

/**
 * @brief Unsubscribe keyboard interrupts.
 * @return Returns 0 upon success, and 1 otherwise.
 */
int (keyboard_unsubscribe_int)();

/**
 * @brief Check if the keyboard interrupt has been concluded.
 * @return Returns 1 if the interrupt is completed and 0 otherwise.
 */
int (keyboard_is_complete)();

/**
 * @brief After the interrupts is done, get a Event struct from 
 * the keyboard interrupt.
 * 
 * @return Event struct with proper data set.
 */
Event (keyboard_get_event)(void);

/**
 * @brief Tranform a makecode into a char.
 * @return Returns char from keyboard (makecode).
 */
char getLetter();

/**
 * @brief Check if a char is a letter and can be used in the game.
 * 
 * @param c Character to test.
 * @return int 1 is the char is alpha, 0 otherwise.
 */
int test_letter(char c);

/**@}*/

#endif
