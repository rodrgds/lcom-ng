#ifndef _LCOM_KEYBOARD_H_
#define _LCOM_KEYBOARD_H_

#include <minix/sysutil.h>
#include <lcom/lcf.h>
#include "i8042.h"
#include "controller/KBC/KBC.h"

#define IRQ_LINE 1 



#define KBC_OUTBUFFER 0x60 ///KBC Address of the output buffer

/**
 * @brief Subscribes keyboard interrupts
 * @return 0 if successful, not 0 if otherwise
 */

int (keyboard_unsubscribe_interrupts)();

/**
 * @brief Unsubscribes keyboard interrupts
 * @return 0 if successful, not 0 if otherwise
 */

int (keyboard_subscribe_interrupts)(uint8_t *bit_no);


#endif
