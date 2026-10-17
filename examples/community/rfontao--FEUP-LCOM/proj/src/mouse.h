#ifndef _MOUSE_H_
#define _MOUSE_H_

#include <lcom/lcf.h>

#include <stdbool.h>
#include <stdint.h>
#include "i8042.h"

/** @defgroup Mouse Mouse
 * @{
 *
 * Functions of the mouse
 */

/**
 * @brief Subscribes the interruptions of the mouse
 * 
 * @param bit_no        Bit of the IRQ line used by the mouse
 * @return              1 on failure
 */
int(mouse_subscribe_int)(int *bit_no);

/**
 * @brief The mouse interrupt handler
 * 
 */
void(mouse_ih)();

/**
 * @brief Unsubscribes the mouse interruptions
 * 
 * @return      1 on failure
 */
int (mouse_unsubscribe_int)();

/**
 * @brief Sends a command cmd to the kbc
 * 
 * @param cmd   command to send to the kbc
 * @return      1 on failure
 */
int(mouse_send_cmd)(uint8_t cmd);

/**
 * @brief Enables mouse remote mode
 * 
 * @return      1 on failure
 */
int(mouse_remote)();

/**
 * @brief Updates the mouse x and y based on the deltax and deltay
 * 
 * @param deltax        Delta x
 * @param deltay        Delta y
 */
void updateMousePosition(int deltax,int deltay);

/**@}*/

#endif
