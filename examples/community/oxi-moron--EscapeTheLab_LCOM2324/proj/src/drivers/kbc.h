#ifndef _LCOM_KBC_H_
#define _LCOM_KBC_H_

#include <minix/sysutil.h>
#include <lcom/lcf.h>
#include "i8024_kbc.h"

/**
 * @brief Subscribes to keyboard interrupts with policies IRQ_REENABLE and IRQ_EXCLUSIVE.
 * @param bit_no Bit number to be used in the keyboard hook ID.
 * @return 0 if successful, non-zero otherwise.
 */
int (keyboard_subscribe_int)(uint8_t *bit_no);

/**
 * @brief Unsubscribes to keyboard interrupts.
 * @return 0 if successful, non-zero otherwise.
 */
int (keyboard_unsubscribe_int)();

/**
 * @brief Handles keyboard interrupts by checking the KBC output buffer and reading its scancode.
 */
void (kbc_ih)();

#endif
