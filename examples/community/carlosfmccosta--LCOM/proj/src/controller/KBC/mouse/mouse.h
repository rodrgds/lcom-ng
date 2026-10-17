/**
 * @file mouse.h
 * @brief Interface for handling i8042 mouse controller operations.
 *
 * This header provides functions for subscribing to mouse interrupts,
 * handling mouse input, reading and writing bytes to the mouse controller,
 * and interpreting mouse packets. It defines the mouse packet structure and
 * utility functions for mouse event processing.
 *
 */
#ifndef __MOUSE_H
#define __MOUSE_H

#include <stdbool.h>
#include <stdint.h>
#include "../i8042.h"

/**
 * @defgroup mouse mouse
 * 
 *
 * @brief Functions for handling mouse input via the i8042 controller.
 * @{
 */

/**
 * @brief Subscribes and enables mouse interrupts.
 *
 * @param bit_no_mouse Address of memory to be initialized with the bit number to be set in the mask returned upon an interrupt.
 * @return 0 on success, 1 otherwise.
 */
int (mouse_subscribe_int)(uint8_t *bit_no_mouse);

/**
 * @brief Unsubscribes mouse interrupts.
 *
 * @return 0 on success, 1 otherwise.
 */
int (mouse_unsubscribe_int)();

/**
 * @brief Enables mouse data reporting.
 *
 * Sends the appropriate command to the mouse to start sending movement and button data.
 *
 * @return 0 on success, 1 otherwise.
 */
int (mouse_nable_data_reporting)();

/**
 * @brief Disables mouse data reporting.
 *
 * Sends command to mouse to stop sending data.
 *
 * @return 0 on success, 1 otherwise.
 */
int (mouse_disable_data_reporting)();

/**
 * @brief Writes a command byte to the mouse.
 *
 * Handles communication with the mouse using 0xD4 via the KBC.
 *
 * @param cmd Command byte to send to the mouse.
 * @return 0 on success, 1 otherwise.
 */
int (mouse_write_cmd)(uint8_t cmd);

/**
 * @brief Mouse interrupt handler.
 *
 * Reads one byte from the output buffer, validates packet sync and constructs a mouse packet from three bytes.
 */
void (mouse_ih)();

/**
 * @brief Updates the current mouse event based on button state or movement.
 *
 * @param pack Mouse packet with the latest data from the interrupt handler.
 */
void (handle_buttons)(struct packet pack);

/**
 * @brief Gets the latest mouse packet.
 *
 * @return A copy of the last completed mouse packet.
 */
struct packet get_mouse_packet();

/** @} */

#endif // __MOUSE_H 
