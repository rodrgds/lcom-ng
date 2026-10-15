#ifndef _LCOM_COM_MANAGER_
#define _LCOM_COM_MANAGER_

#include <lcom/lcf.h>
#include "../../drivers/drivers.h"
#include "../app/app.h"

/**
 * @defgroup com_manager COM Manager
 * @ingroup models
 * @{
 * 
 * @brief Manages data received from the serial port / UART device.
 * 
 * @details This module receives the data and tries to put it inside the com_packet_t object.
 * @details It also processes the current packet when complete.
*/

/** 
 * @defgroup proto Communication Protocol
 * @ingroup com_manager
 * @{
*/

#define PROTO_START	0xFE /**< @brief Message header byte */
#define PROTO_END		0xFF /**< @brief Message trail byte */

/**
 * @struct proto_msg_type_t
 * @brief Defines the type of message communicated.
*/
typedef enum {
	PROTO_NOTIFY_NONE, /**< @brief Notification - Show default */
	PROTO_NOTIFY_WIN, /**< @brief Notification - Player win @details Content must be player's won_coins. */
	PROTO_NOTIFY_LOSS, /**< @brief Notification - Player loss @details Content must be player's bet. */
	PROTO_NOTIFY_DRAW, /**< @brief Notification - Player draw @details Content should be player's won_coins. */
	PROTO_NOTIFY_SURRENDER /**< @brief Notification - Player surrender @details Content should be player's won_coins. */
} proto_msg_type_t;

/** @} */

/**
 * @struct com_packet_t
 * 
 * @brief Structure representing a message received via serial port.
*/
typedef struct {
	proto_msg_type_t type; /**< @brief Message type. */
	uint32_t data; /**< @brief Message content. */
} com_packet_t;

/**
 * @brief Adds a byte received in the UART module to the current packet.
 * 
 * @param byte Byte received
 * @return 1 if the packet if full, 0 otherwise.
*/
int com_add_byte(uint8_t byte);

/**
 * @brief Sends a message to another pc via Serial Port.
 * 
 * @details A message is composed by 3 parts (tecnically 6): a header, a body and a trail.
 * - The header is always 0xFE (254)
 * - The body is divided in 4 bytes, sent sequencially byte by byte. They are sent from LSB to MSB.
 * - The trail is always 0xFF (255) 
 * 
 * @param type Message type
 * @param content Message content
 * @return 0 if successful, non-zero otherwise.
*/
int	com_send_msg(proto_msg_type_t type, uint32_t content);

/**
 * @brief Processes the fully received packet.
 * 
 * @param app Pointer to aplication structure
*/
void com_handle_packet(app_t *app);

/** @} */

#endif /* _LCOM_EV_COM_MANAGER_H_ */
