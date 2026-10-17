#ifndef __MOUSE_H
#define __MOUSE_H

/** @defgroup mouse mouse
 * @{
 *
 * Functions to manage mouse abstraction (getting interrupts and processing input)
 */

#include "i8042.h"
#include "ev_queue.h"

/**
 * @brief To print a packet to the console
 *
 * Writes the given packet to the console in a user friendly way
 *
 * @param packet unsigned char array with the 3 bytes to show
 *
 * @return Return 0 upon success and non-zero otherwise
 */
int print_packet(unsigned char* packet);

/**
 * @brief Subscribes mouse's interrupts
 *
 * @return Return HookID upon success and -1 otherwise
 */
int mouse_subscribe_int();

/**
 * @brief Unsubscribes mouse's interrupts
 *
 * @return Return 0
 */
int mouse_unsubscribe_int();
int check_buffers();
int mouse_send_cmd(unsigned long cmd);
int mouse_send_cmd_raw(unsigned long cmd);
/**
 * @brief Enables mouse in stream mode
 *
 * @return 0 on success, non-zero otherwise
 */
int mouse_enable_stream_mode();
/**
 * @brief Genereate an event associated with the packet received
 *
 * @param events - event queue
 * @param packet - packet received
 * @return none
 */
void mouseHandler(ev_queue_st* events, unsigned char* packet);

/**
 * @brief Process a packet sent by the mouse
 *
 * @param bufferData - data in buffer (used to store sucessive bytes)
 * @param nPacketsReceived - pointer to number of packets received
 * @param returnPacket - final packet to return
 * @return 0/1 dependeing on the state of the reading, -1 to discar current packet
 */
int mouse_movement_handler(unsigned long bufferData, unsigned short *nPacketsReceived, unsigned char *returnPacket);

#endif /* __MOUSE.H */

/** @} end of mouse*/
