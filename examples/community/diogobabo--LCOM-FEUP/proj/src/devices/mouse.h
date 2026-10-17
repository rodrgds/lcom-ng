#ifndef _LCOM_MOUSE_H_
#define _LCOM_MOUSE_H_

#include "i8042.h"
#include <lcom/lcf.h>

#define MIN_SLOPE 1.0

/**
 * @brief checks mouse status for errors
 * 
 */
void (mouse_ih)();
/**
 * @brief subscribes to mouse interrupts
 * 
 */
int (mouse_subscribe_int)(uint8_t *bit_no);
/**
 * @brief unsubscribes to mouse interrupts
 * 
 */
int (mouse_unsubscribe_int)();
/**
 * @brief checks for errors and sends the mouse's command
 * 
 */
int (send_mouse_cmd)(uint8_t cmd);
/**
 * @brief reads and updates the packets
 * 
 */
void (mouse_set_packet)(struct packet *pacote);
/**
 * @brief checks mouse status to see if its input buffer is full
 * 
 */
int (check_ibf_full)();
/**
 * @brief disables mouse's data reporting
 * 
 */
int (mouse_disable_data_reporting)();

#endif
