//
// Created by skidr on 13/10/2019.
//

#ifndef LAB3_KEYBOARD_H
#define LAB3_KEYBOARD_H

#include "lcom/lcf.h"
#include "lcom/lab3.h"
#include "i8042.h"

#ifdef LAB3
int sys_inb_cnt(port_t port, uint32_t *byte);
#else
#define sys_inb_cnt(p,v) sys_inb(p,v)
#endif

/**
 * @file keyboard.h
 * @brief Project Module with functions to manage keyboard
 */

/** @defgroup keyboard keyboard
 * @{
 * Functions for keyboard management
 */

/** @brief Function to count and call sys_inb calls;
 *
 * @param port      port where we are going to write to the value
 * @param value     value we are going to read from port
 * @return          0 upon success
 */
int sys_inb_kbd_count(port_t port, uint32_t *value);

/** @brief Wrapper function for sys_inb with 8bits
 *
 * @param port      port where we are going to write to value
 * @param value     value we are going to read from port
 * @return          0 or OK upon success
 */
int sys_inb_kbd(port_t port, uint8_t *value);

/** @brief Interruptions Handler
 *
 */
void (kbc_ih)(void);

/** @brief Function to subscribe keyboard interruptions
 *
 * @return 0 upon success
 */
int keyboard_subscribe_int(uint32_t *bit_no);

/** @brief Function to unsubscribe keyboard interruptions
 *
 * @return 0 upon success;
 */
int keyboard_unsubscribe_int(void);


/** @brief Polling function
 *
 * @return 0 upon success
 */
int (kbc_poll)(void);

#endif //LAB3_KEYBOARD_H
