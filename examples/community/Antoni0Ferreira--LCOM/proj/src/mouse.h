#ifndef __MOUSE_H
#define __MOUSE_H

#include <lcom/lcf.h>
#include <stdint.h>

/**
 * @defgroup mouse mouse
 * @{
 * 
 * Module for the mouse
 */

uint8_t packet_mouse_ih[3]; // Array for storing the 3 bytes that compose a mouse packet
int counter_mouse_ih;

/**
 * @brief Function responsible for subscribing the interrupts from the mouse
 * 
 * @param bit_no Identifies what bit will be set when an keyboard interruption occurs
 * @return Return 0 upon success and non-zero otherwise 
 */
int (mouse_subscribe_int)(uint8_t *bit_no);

/**
 * @brief Function responsible for unsubscribing the interrupts from the keyboard
 * 
 * @return Return 0 upon success and non-zero otherwise 
 */
int (mouse_unsubscribe_int)();

/**
 * @brief Function Responsible for constructing the mouse packet
 * 
 * @param packet_bytes Array with the 3 bytes that compose the packet
 * @param pp Struct regarding all the information relative to a mouse packet
 */
void (mouse_get_packet)(const uint8_t *packet_bytes, struct packet *pp);

/**
 * @brief Writes command 0xF4 to KBC activating mouse data transfer
 * 
 * @return Return 0 upon success and non-zero otherwise 
 */
int (mouse_enable_data)();

/**
 * @brief Writes command 0xF5 to KBC disabling mouse data transfer
 * 
 * @return Return 0 upon success and non-zero otherwise 
 */
int (mouse_disable_data)();

/**
 * @brief Function responsible for issuing a command to the KBC command register
 * 
 * @param cmd Command to be written
 * @return uint8_t 
 */
int (mouse_issue_command)(uint8_t cmd);

/**
 * @}
 */

#endif /* __MOUSE_H */
