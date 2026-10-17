#ifndef _LCOM_KEYBOARD_H_
#define _LCOM_KEYBOARD_H_

#include <minix/sysutil.h>
#include <lcom/lcf.h>
#include "i8042.h"
#include "kbc.h"



/// @brief Subscribe the keyboard interrupts
/// @param bit_no keyboard_hook_id to bits
/// @return 0 if success, 1 if no success
int (keyboard_subscribe_interrupts)(uint8_t *bit_no);

/// @brief Unsubscribe the kwyboard interrupts
/// @return 0 if success, 1 if no success
int (keyboard_unsubscribe_interrupts)();

/// @brief Interrupt handler of the keyboard
/// @return 
void (kbc_ih)();

#endif /* _LCOM_KEYBOARD_H_ */





