#ifndef _LCOM_KEYBOARD_H_
#define _LCOM_KEYBOARD_H_


#include <minix/sysutil.h>
#include <lcom/lcf.h>
#include "devices/i8042.h"
#include "devices/KBC.h"

/// @brief subscribes the keyboard interrupts
/// @return a 0 in case of success and a 1 in case of error with the system call
int (keyboard_subscribe_interrupts)();

/// @brief unsubscribes the keyboard interrupts
/// @return a 0 in case of success and a 1 in case of error with the system call
int (keyboard_unsubscribe_interrupts)();

/// @brief keyboard interrupt handler, whenever there´s an interrupt from the keyboard it updates the global variable "scancode" to the the latest received scancode
/// @return 
void (keyboard_interrupt_handler)();



#endif
