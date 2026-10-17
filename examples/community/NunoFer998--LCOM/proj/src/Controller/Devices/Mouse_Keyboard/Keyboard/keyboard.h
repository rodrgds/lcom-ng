/**
 * @file keyboard.h
 * @brief Keyboard device driver header file
 * 
 * This header file defines the keyboard driver interface including
 * scan code handling, interrupt management, and subscription functions.
 */

#ifndef _KEYBOARD_H_
#define _KEYBOARD_H_

#include <lcom/lcf.h>
#include <minix/sysutil.h>
#include "../../macros.h"
#include "../../func_aux.h"
#include "../kbc.h"

uint8_t scan_code;  // Global variable to store the current scan code

/**
 * @brief Keyboard interrupt handler
 * 
 * This function handles keyboard interrupts and reads scan codes.
 */
void (kbc_ih)();

/**
 * @brief Gets scan code from keyboard controller
 * 
 * @return 0 on success, non-zero on error
 */
int (get_scancode)();

/**
 * @brief Subscribes to keyboard interrupts
 * 
 * @param mask Pointer to store the interrupt mask
 * @return 0 on success, non-zero on error
 */
int (kb_sub_int)(uint8_t *mask);

/**
 * @brief Unsubscribes from keyboard interrupts
 * 
 * @return 0 on success, non-zero on error
 */
int (kb_unsub_int)();

#endif
