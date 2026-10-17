#ifndef __KBC_H
#define __KBC_H

#include "../utils.h"
#include "i8042.h"

#include <lcom/lcf.h>

// Keyboard IRQ Subscriptions
int subscribe_interrupts_kbd(uint8_t* bit_no);
int unsubscribe_interrupts_kbd();

// Enable Keyboard Interrupts
int kbc_enable_interrupts();

// Functions to read and write to kbc
int readFromStatusKBC(uint8_t* status);
int readFromOutputBuffer(uint8_t* byte);
int writeToKBCPort(uint8_t port, uint8_t data);

#endif /* __KBC_H */
