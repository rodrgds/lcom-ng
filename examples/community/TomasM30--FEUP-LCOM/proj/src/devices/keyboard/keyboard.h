#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "lcom/lcf.h"
#include "kbc.h"
#include "key_map.h"

/** @defgroup keyboard keyboard
 * @brief Functions for interacting with the keyboard
*/

/**
 * @brief Subscribes and enables keyboard interrupts
 * @param bit_no Pointer to the variable where the bit mask will be stored
 * @return 0 upon success, 1 otherwise
*/
int (keyboard_subscribe_int)(uint8_t *bit_no);

/**
 * @brief Unsubscribes keyboard interrupts
 * @return 0 upon success, 1 otherwise
*/
int (keyboard_unsubscribe_int)();

/**
 * @brief Keyboard interrupt handler
*/
void (keyboard_ih)();

/**
 * @brief Restores the Minix IH for the KBC
 * @return 0 upon success, 1 otherwise
*/
int (keyboard_restore)();

#endif 
