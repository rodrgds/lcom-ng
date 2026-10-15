/**
 * @file keyboard
 * @brief This file has the implementation of the functions related to the keyboard.
 */

#ifndef _KEYBOARD_H_
#define _KEYBOARD_H_

#include <lcom/lcf.h>

/**
 * @brief Subscribes to keyboard interrupts with policies IRQ_REENABLE and IRQ_EXCLUSIVE.
 * 
 * @param bit_no Pointer to store the IRQ line bitmask of the keyboard.
 * 
 * @return Returns 0 upon success and non-zero otherwise.
 */
int (keyboard_subscribe_int)(uint8_t* bit_no);

/**
 * @brief Unsubscribes from keyboard interrupts.
 * 
 * @return Returns 0 upon success and non-zero otherwise.
 */
int (keyboard_unsubscribe_int)();

/**
 * @brief Keyboard interrupt handler function.
 * 
 * Checks the status of the keyboard buffer, reads a byte from the buffer if available. 
 * If the byte read is part of a multi-byte scan code, it waits for the next interrupt to read the rest of the scan code.
 * 
 * @return void.
 */
void (kbc_ih)();

#endif
