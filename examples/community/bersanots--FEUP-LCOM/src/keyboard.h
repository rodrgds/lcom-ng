/**
 * @file keyboard.h
 * @author LCOM professors
 * @brief Keyboard device handling
 * @version 0.1
 * @date 2021-01-05
 * 
 * Adapted by Bernardo Santos (bernardo.ots@fe.up.pt)
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#ifndef __KEYBOARD_H
#define __KEYBOARD_H

#include <lcom/lcf.h>

/** @defgroup keyboard Keyboard
 * @{
 *
 * Keyboard device handling
 */

/**
 * @brief Handle keyboard interrupts.
 * 
 */
void(kbc_ih)();

/**
 * @brief Subscribe to keyboard interrupts.
 * 
 * @param bit_no 
 * @return 0 on success, 1 on failure 
 */
int(kbd_subscribe_int)(uint8_t *bit_no);

/**
 * @brief Unsubscribe from keyboard interrupts.
 * 
 * @return 0 on success, 1 on failure 
 */
int(kbd_unsubscribe_int)();

/** @} end of keyboard */

#endif
