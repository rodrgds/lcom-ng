#ifndef _LCOM_KEYBOARD_H_
#define _LCOM_KEYBOARD_H_

#include <minix/sysutil.h>
#include "i8042.h"
#include <lcom/lcf.h>
#include "KBC.h"

/**
 * @brief Function that subscribes the keyboard interrupts.
 * @param bit_no Variable to store the bit mask of the keyboard interruptions.
 * @return 0 if the keyboard interrupts were successfully subscribed, 1 otherwise.
 */
int (keyboard_subscribe_int)(uint8_t *bit_no);

/**
 * @brief Function that unsubscribes the keyboard interrupts.
 * @return 0 if the keyboard interrupts were successfully unsubscribed, 1 otherwise.
 */
int (keyboard_unsubscribe_int)();

/**
 * @brief Function that reads the current scancode of the keyboard from the KBC.
 */
void (kbc_ih)();

#endif
