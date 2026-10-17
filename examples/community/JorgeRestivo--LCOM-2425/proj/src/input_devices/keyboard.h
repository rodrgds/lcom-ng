/**
 * @file keyboard.h
 * @brief Keyboard module
 *  */

#ifndef _LCOM_KEYBOARD_H_
#define _LCOM_KEYBOARD_H_

#include <minix/sysutil.h>
#include <lcom/lcf.h>
#include "kbc.h"

/**
 * @brief Stores the latest keyboard scancode read.
 */
extern uint8_t scancode;

/**
 * @brief Subscribes keyboard interrupts.
 * @param bit_no Pointer to variable to store the interrupt bit number.
 * @return 0 on success, non-zero otherwise.
 */
int (keyboard_subscribe_int)(uint8_t *bit_no);

/**
 * @brief Unsubscribes keyboard interrupts.
 * @return 0 on success, non-zero otherwise.
 */
int (keyboard_unsubscribe_int)();

/**
 * @brief Keyboard interrupt handler. Reads scancode from the output buffer.
 */
void (kbc_ih)();

/**
 * @brief Restores the keyboard to its default state.
 * @return 0 on success, non-zero otherwise.
 */
int (keyboard_restore)();

#endif
