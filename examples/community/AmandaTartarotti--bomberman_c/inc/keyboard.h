#ifndef _LCOM_KEYBOARD_H_
#define _LCOM_KEYBOARD_H_

#include <lcom/lcf.h>

/**
     * @brief Subscribes and enables Keyboard interrupts. 
     * 
     * @param bit_no address of memory to be initialized with the bit number to be set in the mask returned upon an interrupt
     * 
     * @return Return 0 upon success and non-zero otherwise
**/
int kbd_subscribe_int(uint8_t *bit_no);
/**
     * @brief Unsubscribes Keyboard interrupts. 
     * 
     * @return Return 0 upon success and non-zero otherwise
**/
int kbd_unsubscribe_int();
/**
     * @brief Handles keyboard interrupts.
     * 
     * Reads the status register and the output buffer (OB).
     * If there was some error, the byte read from the OB should be discarded.
     * 
**/
void (kbc_ih)();

#endif
