/** 
 * @file mouse.h
 * @brief Header file for @ref mouse.c
 * 
 * This file declares all the functions to interact with the kbc's mouse.
 */


#ifndef _MOUSE_H_
#define _MOUSE_H_

#include <lcom/lcf.h>
#include "i8042.h"
#include "kbc.h"

/**
 * @brief Subscribes the mouse interruptions with the payload on bit bit_no
 * 
 * Uses the IRQ_EXCLUSIVE tag so MINIX does not steal the interrupts.
 * 
 * @param bit_no The bit position to set the payload
 * @return 0 in case of success, 1 otherwise
 */
int (mouse_subscribe_int_exclusive)(uint8_t *bit_no);

/**
 * @brief Unsubscribes the mouse interruptions
 * 
 * @return 0 in case of success, 1 otherwise
 */
int (mouse_unsubscribe_int)();

/**
 * @brief Getter for the @ref mouse_packet_byte static variable
 * 
 * @return The latest mouse_packet_byte read
 */
uint8_t (mouse_get_packet_byte)();

/**
 * @brief Enables the data reporting of the mouse' stream mode
 * 
 * Tries to write the necessary commands to the kbc and then waiting for 20ms for a response. Tries this 5 times.
 * 
 * @return 0 in case of success, 1 otherwise
 */
int (mouse_stream_enable_data_reporting)();

/**
 * @brief Disables the data reporting of the mouse' stream mode
 * 
 * Tries to write the necessary commands to the kbc and then waiting for 20ms for a response. Tries this 5 times.
 * 
 * @return 0 in case of success, 1 otherwise
 */
int (mouse_stream_disable_data_reporting)();

/**
 * @brief Assembles the packet bytes into a packet structure
 * 
 * @param packet_bytes The array of the 3 packet bytes
 * @return The assembled packet
 */
struct packet (mouse_assemble_packet)(uint8_t packet_bytes[3]);


#endif /* _MOUSE_H_ */
