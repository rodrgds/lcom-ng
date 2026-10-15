/**
 * @file keyboard.h
 * 
 * @brief This file contains the definition of the functions to configure and interact with the keyboard.
*/

#ifndef _LCOM_KEYBOARD_H_
#define _LCOM_KEYBOARD_H_

#define KEYBOARD_MASK BIT(1)

#include <minix/sysutil.h>
#include <lcom/lcf.h>
#include "i8042.h"
#include "KBC.h"

/**
 * @brief This function subscribes keyboard interrupts, using the sys_irqsetpolicy() function.
 * 
 * @return int 0 if no errors occurred, 1 otherwise
*/
int (keyboard_subscribe_interrupts)();

/**
 * @brief This function unsubscribes keyboard interrupts, using the sys_irqrmpolicy() function
 * and the hook_id_keyboard variable, fetched from the keyboard_subscribe_interrupts() function.
 * 
 * @return int 0 if no errors occurred, 1 otherwise
*/
int (keyboard_unsubscribe_interrupts)();


/**
 * @brief This function handles keyboard interrupts, using the read_KBC_output() function to
 * get the current byte information, storing it in the scancode variable.
*/
void (kbc_ih)();

/**
 * @brief This function restores the KBC by enabling again the interrupts.
 * 
 * @return int 0 if no errors occurred, 1 otherwise
*/
int (keyboard_restore)();

#endif
