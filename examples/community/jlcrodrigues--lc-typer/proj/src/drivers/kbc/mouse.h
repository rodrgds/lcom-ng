#ifndef MOUSE_H
#define MOUSE_H

/** @defgroup kbc mouse
 * @{
 *
 * Functions related to the moue driver.
 */

#include <lcom/lcf.h>

#include "i8042.h"
#include "event.h"

/**
 * @brief Subscribe mouse interrupts and disable Minix's default handler.
 * @param bit_no Receives the bit that will be set for mouse interrupts.
 * @return Returns 0 upon success, and 1 otherwise.
 */
int (mouse_subscribe_int)(uint8_t *bit_no);

/**
 * @brief Unsubscribe mouse interrupts.
 * @return Returns 0 upon success, and 1 otherwise.
 */
int (mouse_unsubscribe_int)();

/**
 * @brief Enable and disable mouse data reporting.
 * @param set 1 to enable data reporting and 0 to disable it.
 * @return Returns 0 upon success, and 1 otherwise.
 */
int (mouse_set_data_reporting)(bool set);

/**
 * @brief Parse the packets received from the mouse.
 * @return Returns a struct packet created from the parsed packets.
 */
struct packet (parse_packets)();

/**
 * @brief Send a command to the mouse.
 * @param cmd Mouse command.
 * @return Returns 0 upon success, and 1 otherwise.
 */
int (send_mouse_cmd)(uint32_t cmd);

/**
 * @brief Read a command from the mouse.
 * @param byte Pointer to store the read information.
 * @return Returns 0 upon success, and 1 otherwise.
 */
int (mouse_read)(uint8_t *byte);

/**
 * @brief Get the amount of packets read.
 * @return The amount of read packets varies from 0 to 3 (complete).
 */
int (get_packets_count)();

/**
 * @brief If all packets have been set, it generates an Event with mouse data.
 * 
 * @return Event struct with proper data set.
 */
Event (mouse_get_event)(void);

/**@}*/

#endif
