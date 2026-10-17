#ifndef _LCOM_MOUSE_H_
#define _LCOM_MOUSE_H_

#include <lcom/lcf.h>
#include "../keyboard/i8042.h"
#include "../keyboard/kbc.h"

/**
 * @brief Subscribes mouse interrupts
 * 
 * @param bit_no Pointer to store the interrupt bit mask
 * 
 * @return 0 on success, 1 on failure
 */
int (mouse_subscribe_int)(uint8_t *bit_no);

/**
 * @brief Unsubscribes mouse interrupts
 * 
 * @return 0 on success, 1 on failure 
 */
int (mouse_unsubscribe_int)();

/**
 * @brief Disables mouse interrupts
 * 
 * @return 0 on success, 1 on failure
 */
int (mouse_disable)();

/**
 * @brief Enables mouse interrupts
 * 
 * @return 0 on success, 1 on failure
 */
int (mouse_enable)();

/**
 * @brief Mouse interrupt handler
 */
void (mouse_ih)();

/**
 * @brief Sends a command to the mouse
 * 
 * @param cmd The command byte to send
 * 
 * @return 0 on success, 1 on failure
 */
int (mouse_write)(uint8_t cmd);

/**
 * @brief Parses the complete mouse packet and resets the buffer 
 */
void (mouse_packet_handler)();

/**
 * @brief Synchronizes the mouse packet
 */
void (mouse_sync_packet)();


#endif
