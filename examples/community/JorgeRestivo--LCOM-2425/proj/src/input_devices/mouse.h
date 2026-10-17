/**
 * @file mouse.h
 * @brief Mouse module
 */

#ifndef MOUSE_H
#define MOUSE_H

#include <lcom/lcf.h>
#include "constants.h"
#include "types.h"
// Mouse IRQ
#define IRQ_MOUSE 12

// Mouse commands
#define ENABLE_DATA_REPORT 0xF4
#define DISABLE_DATA_REPORT 0xF5
#define WRITE_BYTE_TO_MOUSE 0xD4

// Mouse responses
#define ACK 0xFA
#define NACK 0xFE
#define ERROR 0xFC

// I/O Ports
#define STAT_REG 0x64
#define KBC_CMD_REG 0x64
#define OUT_BUF 0x60
#define IN_BUF 0x60

// Status Register Bits
#define OBF BIT(0)
#define IBF BIT(1)
#define AUX BIT(5)
#define TO_ERR BIT(6)
#define PAR_ERR BIT(7)

#define DELAY_US 20000
#define MOUSE_OUT_BUF 0x60


extern Vec_pos cursor_pos;

/**
 * @brief Subscribes mouse interrupts.
 * @param bit_no Pointer to variable to store the interrupt bit number.
 * @return 0 on success, non-zero otherwise.
 */
int mouse_subscribe_int(uint8_t *bit_no);

/**
 * @brief Unsubscribes mouse interrupts.
 * @return 0 on success, non-zero otherwise.
 */
int mouse_unsubscribe_int(void);

/**
 * @brief Disables mouse data reporting.
 * @return 0 on success, non-zero otherwise.
 */
int mouse_disable_data_reporting(void);

/**
 * @brief Mouse interrupt handler. Reads bytes from the mouse and assembles packets.
 */
void proj_mouse_ih(void);

/**
 * @brief Checks if a mouse packet is ready to be processed.
 * @return true if a packet is ready, false otherwise.
 */
bool mouse_packet_ready(void);

/**
 * @brief Gets the latest mouse packet.
 * @return The latest Mousepacket structure.
 */
struct packet get_mouse_packet(void);

/**
 * @brief Parses the mouse packet bytes into a packet structure.
 * @param bytes Array of 3 bytes from the mouse.
 * @param pp Pointer to the packet structure to fill.
 */
void parse_mouse_packet(uint8_t bytes[3], struct packet *pp);

/**
 * @brief Writes a command to the mouse.
 * @param cmd Command byte to write.
 * @return 0 on success, non-zero otherwise.
 */
int (mouse_write_cmd)(uint8_t cmd);

/**
 * @brief Disables mouse data reporting (duplicate, for compatibility).
 * @return 0 on success, non-zero otherwise.
 */
int(mouse_disable_data_reporting)();

/**
 * @brief Mouse gesture recognition states.
 */
typedef enum {
  INIT,      /**< Initial state */
  DRAW_UP,   /**< Drawing the first line */
  VERTEX,    /**< Between strokes */
  DRAW_DOWN, /**< Drawing the second line */
  COMPLETE   /**< Gesture complete */
} gesture_state_t;

#define INIT_STATE INIT
#define COMPLETE_STATE COMPLETE

#endif
