#ifndef _LCOM_KEYBOARD_H_
#define _LCOM_KEYBOARD_H_

#include <minix/sysutil.h>
#include <lcom/lcf.h>
#include "i8042.h"

/**
 * @file keyboard.h
 * @brief Declares functions and variables for handling keyboard input via interrupts.
 *
 * Provides subscription management for keyboard interrupts and 
 * includes handlers for interpreting scan codes.
 */

int (keyboard_subscribe_int)(uint8_t *bit_no);
int (keyboard_unsubscribe_int)();
void (kbc_ih)();
int(read_out)();

extern uint8_t scan_code[2];    // added to implement a playable character
extern uint8_t size;            // same

#endif
