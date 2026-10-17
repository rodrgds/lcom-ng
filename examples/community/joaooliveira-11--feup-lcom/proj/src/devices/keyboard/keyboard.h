#ifndef _KEYBOARD_PROJECT
#define _KEYBOARD_PROJECT

#include <minix/sysutil.h>
#include <lcom/lcf.h>
#include "../util/KBC.h"


///@brief Subscribes to the keyboard interrupts.
///@note This function subscribes to the keyboard interrupts by setting the appropriate bit in the mask and calling sys_irqsetpolicy to register the interrupt handler.
///@param mask Pointer to the mask that will hold the keyboard interrupt bit.
///@return 0 if the subscription is successful, otherwise returns 1.
int (keyboard_subscribe_int)(uint8_t *bit_no);


///@brief Subscribes to the keyboard interrupts.
///@note This function subscribes to the keyboard interrupts by setting the appropriate bit in the mask and calling sys_irqsetpolicy to register the interrupt handler.
///@param mask Pointer to the mask that will hold the keyboard interrupt bit.
///@return 0 if the subscription is successful, otherwise returns 1.
int (keyboard_unsubscribe_int)();


 ///@brief Subscribes to the keyboard interrupts.
 ///@note This function subscribes to the keyboard interrupts by setting the appropriate bit in the mask and calling sys_irqsetpolicy to register the interrupt handler.
 ///@param mask Pointer to the mask that will hold the keyboard interrupt bit.
 ///@return 0 if the subscription is successful, otherwise returns 1.
void (kbc_ih)();

#endif
