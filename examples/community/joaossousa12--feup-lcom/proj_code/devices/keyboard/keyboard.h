/**
 * @file keyboard
 * 
 * @brief Header file for keyboard-related functions and definitions.
 * 
 * @author João Pedro Sousa, Bernardo Pinto, Carlos Madaleno
 * 
 * @date 28/05/2023
*/

#ifndef _LCOM_KEYBOARD_H_
#define _LCOM_KEYBOARD_H_

#include <minix/sysutil.h>
#include <lcom/lcf.h>
#include <stdio.h>
#include "i8042.h"
#include "KBC.h"

/**
 * @brief Subscribes to keyboard interrupts.
 * 
 * @return  Returns the return value of sys_irqsetpolicy(). On success, it returns the bitmask representing the subscribed IRQ line. 
 * On failure, it returns a negative value.
*/

int (keyboard_subscribe_interrupts)();

/**
 * @brief Unsubscribes keyboard interrupts.
 * 
 * @return Returns 0 on success, non-zero if there was an error unsubscribing the interrupts.
*/

int (keyboard_unsubscribe_interrupts)();

/**
 * @brief Keyboard interrupt handler.
*/

void (kbc_ih)();

/**
 * @brief Restores the keyboard's interrupt configuration.
 * 
 * @return 0 on success, non-zero if there was an error restoring the keyboard's interrupt configuration.
*/

int (keyboard_restore)();

#endif
