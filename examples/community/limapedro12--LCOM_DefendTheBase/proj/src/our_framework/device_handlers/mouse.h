#ifndef MOUSE_H
#define MOUSE_H

#include <lcom/lcf.h>
#include "i8042.h"
#include "kbd.h"

#define MOUSE_KEY 29
#define IRQ_SET_MOUSE BIT(MOUSE_KEY)

/**
 * @brief function that subscribes mouse interrupts
 * 
 * @return int 0 if success, 1 otherwise
 */
int mouse_subscribe_interrupt();

/**
 * @brief function that unsubscribes mouse interrupts
 * 
 * @return int 0 if success, 1 otherwise
 */
int mouse_unsubscribe_interrupt();

/**
 * @brief function that transforms the 3 bytes given by the interrupts of the mouse into a packet 
 * 
 * @param byte array of 3 bytes
 * @param pp pointer to the packet that will be filled
 */
int parse_to_packet(u8_t byte[3], struct packet* pp);

/**
 * @brief function that enables mouse data reporting
 * 
 * @return int 0 if success, 1 otherwise
 */
int mouse_enable_data_reporting_ours();

/**
 * @brief function that disables mouse data reporting
 * 
 * @return int 0 if success, 1 otherwise
 */
int mouse_disable_data_reporting();


#endif
