#pragma once

#include "events/events.h"

#include "game/model/cell_background.h"

typedef enum {
  UNKNOWN,
  HANDSHAKE_TRANSMITTED,
  HANDSHAKE_RECEIVED,

  WORD_TRANSMITTED
} protocol_step;

/**
 * @brief Initializes protocol
 * 
 */
void (protocol_init)();

/**
 * @brief Cleans up protocol
 * 
 */
void (protocol_cleanup)();

/**
* @brief Sends the handshake byte to the target
*/
void (protocol_send_handshake)(char *target);

/**
 * @brief Handles acknowledgment 
 * 
 */
void (protocol_handle_ack)();

/**
* @brief Handles a received packet
*/
void (protocol_handle_packet)(struct event *ev);

/**
* @brief Handles the handshake byte
*/
void (protocol_handle_handshake)();
void (protocol_send_submission)(cell_background *bg, size_t n);
