#ifndef _LCOM_KEYBOARD_H_
#define _LCOM_KEYBOARD_H_

#include <minix/sysutil.h>
#include <lcom/lcf.h>
#include <stdint.h>
#include "KBC.h"

int (keyboard_subscribe_int)(uint8_t *irq_set);
int (keyboard_unsubscribe_int)();

void (kbc_ih)(); 	    // Interrupt handler for the keyboard controller

int (restore_interrupts)();

#endif
