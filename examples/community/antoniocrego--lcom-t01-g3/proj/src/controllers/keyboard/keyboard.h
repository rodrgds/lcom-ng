#ifndef _LCOM_KEYBOARD_H_
#define _LCOM_KEYBOARD_H_

#include <minix/sysutil.h>
#include <lcom/lcf.h>

/// @brief Subscribes the interrupts for keyboard
/// @param bit_no The bit of keyboard's hook id
/// @return Returns 0 on success and 1 otherwise
int (keyboard_subscribe_interrupts)(uint8_t *bit_no);

/// @brief Unsubscribes the interrupts for keyboard
/// @return Returns 0 on success and 1 otherwise
int (keyboard_unsubscribe_interrupts)();

#endif
