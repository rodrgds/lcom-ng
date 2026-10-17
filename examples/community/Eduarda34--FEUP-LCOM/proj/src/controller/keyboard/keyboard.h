#ifndef _LCOM_KEYBOARD_H_
#define _LCOM_KEYBOARD_H_

#include <lcom/lcf.h>
#include "i8042.h"
#include "kbc.h"

/**
 * @brief Subscribes keyboard interrupts
 * 
 * @param bit_no Pointer to store the interrupt bit mask
 *
 *  @return 0 on success, 1 on failure
 */
int (keyboard_subscribe_int)(uint8_t *bit_no);

/**
 * @brief Unsubscribes keyboard interrupts
 * 
 * @return 0 on success, 1 on failure
 */
int (keyboard_unsubscribe_int)();

/**
 * @brief Keyboard interrupt handler
 */
void (kbc_ih)();

/**
 * @brief Restores the keyboard controller command byte to enable interrupts
 * 
 * @return 0 on success, 1 on failure
 */
int (keyboard_restore)();

/**
 * @brief Waits for the ESC key to be pressed
 * 
 * @return 0 on success, 1 on failure
 */
int (wait_ESC)();

#endif
