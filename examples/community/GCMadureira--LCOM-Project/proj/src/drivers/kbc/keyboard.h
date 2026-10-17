/** 
 * @file keyboard.h
 * @brief Header file for @ref keyboard.c
 * 
 * This file declares all the functions to interact with the kbc's keyboard.
 */


#ifndef _KEYBOARD_H_
#define _KEYBOARD_H_

#include <lcom/lcf.h>
#include <lcom/utils.h>
#include <lcom/timer.h>

#include "i8042.h"
#include "kbc.h"

/**
 * @brief Subscribes the keyboard interruptions with the payload on bit bit_no
 * 
 * Uses the IRQ_EXCLUSIVE tag so MINIX does not steal the interrupts.
 * 
 * @param bit_no The bit position to set the payload
 * @return 0 in case of success, 1 otherwise
 */
int (keyboard_subscribe_int_exclusive)(uint8_t *bit_no);

/**
 * @brief Unsubscribes the keyboard interruptions
 * 
 * @return 0 in case of success, 1 otherwise
 */
int (keyboard_unsubscribe_int)();

/**
 * @brief Checks if the provided scancode is a breakcode
 * 
 * @param scancode The scancode to check
 * @return true if the scancode is a breakcode, false if it is a makecode
 */
bool (keyboard_is_breakcode)(uint8_t scancode);

/**
 * @brief Getter for the @ref scancode static variable
 * 
 * @return The latest scancode read
 */
uint8_t (keyboard_get_scancode)();

/**
 * @brief Reenable the keyboard interrupts if they were disabled
 * 
 * @return 0 in case of success, 1 otherwise
 */
int (keyboard_reenable_interrupts)();


#endif /* _KEYBOARD_H_ */
