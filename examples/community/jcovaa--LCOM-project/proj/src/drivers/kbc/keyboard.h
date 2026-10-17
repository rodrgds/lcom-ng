/**
 * @file keyboard.h
 * @brief Functions for handling keyboard interrupts and communication.
 */

#ifndef _LCOM_KEYBOARD_H
#define _LCOM_KEYBOARD_H

#include <minix/sysutil.h>
#include <lcom/lcf.h>
#include "i8042.h"

/**
 * @brief Subscribes keyboard interrupts.
 * 
 * @param bit_no Pointer to a variable where the bit number will be stored.
 * @return 0 on success, non-zero otherwise.
 */
int (keyboard_subscribe_interrupts)(uint8_t* bit_no);

/**
 * @brief Unsubscribes keyboard interrupts.
 * 
 * @return 0 on success, non-zero otherwise.
 */
int (keyboard_unsubscribe_interrupts)();

/**
 * @brief Keyboard interrupt handler.
 */
void (kbc_ih)();

/**
 * @brief  Restore the KBC configuration to MINIX's default.
 * 
 * @return 0 on success, non-zero otherwise.
 */
int (restore_interrupts)();

#endif
