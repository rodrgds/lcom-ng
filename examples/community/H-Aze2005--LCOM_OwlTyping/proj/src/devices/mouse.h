/**
 * @file mouse.h
 * @brief Mouse device interface and interrupt handling.
 *
 * Declares functions and variables for subscribing to, unsubscribing from,
 * and handling mouse interrupts, as well as managing mouse state and packet data.
 */

#ifndef _MOUSE_H_
#define _MOUSE_H_

#include <stdint.h>
#include <lcom/lcf.h>
#include "i8042.h"

/**
 * @brief Hook ID for mouse interrupts.
 */
int mouse_hook_id;

/**
 * @brief Index of the current byte being read from the mouse packet.
 */
int current_mouse_byte;

/**
 * @brief Structure holding the latest mouse packet data.
 */
struct packet mouse_packet;

/**
 * @brief Current X position of the mouse cursor.
 */
int cursor_x;

/**
 * @brief Current Y position of the mouse cursor.
 */
int cursor_y;

/**
 * @brief Subscribes mouse interrupts.
 * @param bit_no Address of variable to store the hook id bit number.
 * @return 0 on success, non-zero otherwise.
 */
int (subscribe_mouse)(uint8_t *bit_no);

/**
 * @brief Unsubscribes mouse interrupts.
 * @return 0 on success, non-zero otherwise.
 */
int (unsubscribe_mouse)();

/**
 * @brief Disables mouse data reporting.
 * @return 0 on success, non-zero otherwise.
 */
int (mouse_disable_data_reporting)();

/**
 * @brief Mouse interrupt handler.
 */
void (mouse_int_h)();

#endif
