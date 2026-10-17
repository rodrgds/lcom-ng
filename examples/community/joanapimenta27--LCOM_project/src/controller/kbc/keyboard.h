/**
 * @file keyboard.h
 * @brief Header file for keyboard related functions and definitions.
 */

#ifndef _LCOM_KEYBOARD_H_
#define _LCOM_KEYBOARD_H_

#include <minix/sysutil.h>
#include <lcom/lcf.h>
#include "KBC.h"

/**
 * @brief Subscribes and enables keyboard interrupts.
 * 
 * @param bit_no Address of memory to be initialized with the
 *         bit number to be set in the mask returned upon an interrupt.
 * @return Returns 0 upon success and non-zero otherwise.
 */
int (keyboard_subscribe)(uint8_t *bit_no);

/**
 * @brief Unsubscribes keyboard interrupts.
 * 
 * @return Returns 0 upon success and non-zero otherwise.
 */
int (keyboard_unsubscribe)();

/**
 * @brief Keyboard interrupt handler.
 * 
 * Reads the scancode from the KBC output port and stores it in a global variable.
 */
void (kbc_ih)();

#endif
