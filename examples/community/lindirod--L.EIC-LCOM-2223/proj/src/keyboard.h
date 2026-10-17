/**
 * @file keyboard
 * @author Diogo Pintado, Gabriel Machado Jr., José Costa, Linda Rodrigues
 * 
 *
 * @brief Contains functions related to the keyboard.
 * 
 */

#ifndef _LCOM_KEYBOARD_H_
#define _LCOM_KEYBOARD_H_


#include <lcom/lcf.h>
#include <minix/sysutil.h>
#include <stdint.h>
#include "i8042.h"


/**
 * @brief Subscribes a keyboard interrupt.
 * 
 * @param bit_no Address of memory to be initialized with the bit number to be set in the mask returned upon an interrupt
 * 
 * @return Return 0 upon success and non-zero if otherwise
 */
int (kbc_subscribe_int)(uint8_t *bit_no);


/**
 * @brief Unsubscribes a keyboard interrupt
 * 
 * @return Return 0 upon success and non-zero if otherwise
 * 
 */
int (kbc_unsubscribe_int)();


/**
 * @brief Reads from the port and transforms it into an 8 bit value
 * 
 * @param port The port to be read
 * @param value Address of memory to be initialized with the value that was read
 * 
 * @return Return 0 upon success and non-zero if otherwise
 * 
 */
int (util_sys_inb)(int port, uint8_t *value);


/**
 * @brief Keyboard interrupt handler
 * 
 */
void (kbc_ih)();


/**
 * @brief Print scancodes
 * 
 */
void kbc_scancodes();

/**
 * @brief Polls the keyboard
 * 
 * @return Return 0 upon success and non-zero otherwise 
 */
int kbc_poll();

#endif /*_LCOM_KEYBOARD_H_*/
