#ifndef __PROJ_MOUSE_H
#define __PROJ_MOUSE_H

#include "controller/graphics/vbe.h"
#include "controller/kbc/kbc.h"
#include <stdbool.h>

/** @defgroup mouse mouse
 * @{
 *
 * Functions for interacting with the Mouse.
 */

/**
 * @brief Stores mouse state information
 *
 * This structure maintains the current state of the mouse, including
 * button states and cursor position coordinates.
 *
 * @param rb Right button state (true if pressed, false otherwise)
 * @param lb Left button state (true if pressed, false otherwise)
 * @param x Current x-coordinate of the mouse cursor
 * @param y Current y-coordinate of the mouse cursor
 */
typedef struct {
  bool rb, lb;
  int16_t x, y;
} mouse_info_t;

/**
 * @brief Initializes the mouse with a specific position
 *
 * @param x Initial x-coordinate of the mouse cursor
 * @param y Initial y-coordinate of the mouse cursor
 *
 * @return Return 0 upon success and non-zero if the coordinates are invalid
 */
int(init_mouse)(int16_t x, int16_t y);

/**
 * @brief Getter for the mouse index
 *
 * @return Return the current mouse index
 */
uint8_t(mouse_get_index)(void);

/**
 * @brief Gets the current mouse information
 *
 * @return The current mouse_info_t structure containing position and button states
 */
mouse_info_t(mouse_get_info)(void);

/**
 * @brief Subscribes mouse interrupts
 *
 * @param bit_no Address of memory position to be filled with the hook_id
 * @return Return 0 upon success and non-zero otherwise
 */
int(mouse_subscribe_int)(uint8_t *bit_no);

/**
 * @brief Unsubscribes mouse interrupts
 *
 * @return Return 0 upon success and non-zero otherwise
 */
int(mouse_unsubscribe_int)(void);

/**
 * @brief Writes a command to the mouse
 *
 * Sends a command to the mouse by writing the appropriate bytes to the KBC.
 * Retries up to a maximum number of attempts if the mouse does not acknowledge the command.
 *
 * @param cmd The command to be sent to the mouse
 * @return Return 0 upon success, or 1 if an error occurs or the mouse does not acknowledge the command
 */
int(mouse_write_cmd)(uint8_t cmd);

/**
 * @brief Synchronizes the driver with the mouse packet stream
 */
void(mouse_sync)(void);

/**
 * @brief Parses a mouse packet accoarding to the documentation
 *
 * @return Returns the parsed struct mouse packet
 */
struct packet(mouse_parse_packet)(void);

/**
 * @brief Updates mouse information based on the latest packet
 *
 * @param pp The parsed mouse packet containing movement and button data
 */
void(mouse_update_info)(struct packet pp);

/**
 * @brief Mouse interrupt handler
 *
 * Reads the byte from the KBC output buffer
 */
void(mouse_ih)(void);

/**@}*/

#endif /* __PROJ_MOUSE_H */
