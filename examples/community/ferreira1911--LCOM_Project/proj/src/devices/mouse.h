#ifndef MOUSE_H
#define MOUSE_H

#include <stdint.h>
#include <stdbool.h>
#include <lcom/lcf.h>   
#include "kbc.h"

#define MOUSE_IRQ 12 /**< @brief Mouse IRQ line */
#define KBC_WRITE_MOUSE 0xD4 /**< @brief Write command for the mouse */

#define MOUSE_ACK 0xFA  /**< @brief Mouse Acknowledgment byte */
#define MOUSE_NACK 0xFE /**< @brief Mouse Error byte */
#define ERROR 0xFC

#define MOUSE_DATA_STREAM_MODE 0xEA    /**< @brief Set Stream Mode */
#define MOUSE_DATA_REMOTE_MODE 0xEA    /**< @brief Set Remote Mode */
#define MOUSE_DATA_REPORT_ENABLE 0xF4  /**< @brief Enable Data Reporting */
#define MOUSE_DATA_REPORT_DISABLE 0xF5 /**< @brief Disable Data Reporting */

/* Control Byte - Byte 1 */
#define MOUSE_OVERFLOW_Y BIT(7) /**< @brief Y Overflow */
#define MOUSE_OVERFLOW_X BIT(6) /**< @brief X Overflow */
#define MOUSE_SIGNAL_Y BIT(5)   /**< @brief Y Sign */
#define MOUSE_SIGNAL_X BIT(4)   /**< @brief X Sign */
#define MOUSE_CONTROL_BYTE BIT(3)       /**< @brief Control Byte */
#define MOUSE_MIDDLE_BUTTON BIT(2)         /**< @brief Middle Button */
#define MOUSE_RIGHT_BUTTON BIT(1)         /**< @brief Right Button */
#define MOUSE_LEFT_BUTTON BIT(0)         /**< @brief Left Button */

int (mouse_subscribe_int)(uint8_t *bit_no);
int (mouse_unsubscribe_int)();
int (write_mouse_cmd)(uint8_t cmd);
int (read_mouse_data)(uint8_t *byte);

void (mouse_ih)();
void (sync_bytes)();
void (parse_packet)(struct packet *pp);

bool mouse_packet_ready();
void get_mouse_packet(struct packet *pp);
void reset_mouse_packet();

#endif 

