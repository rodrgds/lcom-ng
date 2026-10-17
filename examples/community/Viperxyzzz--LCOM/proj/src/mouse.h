#ifndef MOUSE_H
#define MOUSE_H

#include <lcom/lcf.h>
#include "i8042.h"



/**
 * @brief parsed content of the mouse
 * 
 */
extern struct packet mouse_packets;

/**
 * @brief indicates if we're done parsing a packet
 * 
 */
extern bool complete;

/**
 * @brief subscribes mouse interrupts
 *
 * @return int 0 in case it goes successfully,1 otherwise 
 */
int (mouse_subscribe_int)();
/**
 * @brief unsubscribes mouse interrupts
 * 
 * @return int 0 in case it goes successfully,1 otherwise
 */
int (mouse_unsubscribe_int)();

/**
 * @brief Reads the output buffer of the mouse
 * 
 */
void (mouse_ih)();

/**
 * @brief Reads the status register and the output buffer, verifying if everything we're doing is valid
 *
 * @return int 0 in case it goes successfully,1 otherwise 
 */
int (analize_buffer)();

/**
 * @brief saves the packets we're currently reading
 * 
 * @return int 0 in case it goes successfully,1 otherwise
 */
int (process_packet)();

/**
 * @brief parses the packets to @mouse_packets
 * 
 */
void (parse_packet)();

/**
 * @brief sends a command to the mouse
 * 
 * @return int 0 in case it goes successfully,1 otherwise
 */
int (send_cmd_mouse)(uint8_t cmd);


#endif
