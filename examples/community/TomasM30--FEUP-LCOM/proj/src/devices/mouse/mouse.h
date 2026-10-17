#ifndef MOUSE_H
#define MOUSE_H

#include <lcom/lcf.h>
#include "kbc.h"
#include "../video/video.h"
#include "../../sprites/sprite.h"

/** @defgroup mouse mouse
 *  @brief Functions for interacting with the mouse
*/

extern Sprite *cursor; /**< @brief Cursor's sprite */

/**
 * @brief Subscribes and enables mouse interrupts
 * @param bit_no Pointer to the variable where the bit mask will be stored
 * @return 0 upon success, 1 otherwise
*/
int (mouse_subscribe_int)(uint8_t *bit_no);

/**
 * @brief Unsubscribes mouse interrupts
 * @return 0 upon success, 1 otherwise
*/
int (mouse_unsubscribe_int)();

/**
 * @brief Mouse interrupt handler
*/
void (mouse_ih)();

/**
 * @brief Writes a command to the mouse
 * @param cmd Command to write
 * @return 0 upon success, 1 otherwise
*/
int (mouse_write_cmd)(uint8_t cmd);

/**
 * @brief Syncs the mouse's packet's bytes
*/
void (mouse_sync_bytes)();

/**
 * @brief Parses the mouse's packet 
*/
void (mouse_parse_packet)();

/**
 * @brief Updates the mouse's position
*/
void (mouse_update_position)();

/**
 * @brief Draws the mouse's cursor
 * @return 0 upon success, 1 otherwise
*/
int (mouse_draw_cursor)();

/**
 * @brief Checks if the mouse is over a button
 * @param button Button to check
 * @return True if the mouse is over the button, false otherwise
*/
bool (mouse_over_button)(Sprite *button);

/**
 * @brief Checks if the mouse clicked a button
 * @param button Button to check
 * @return True if the mouse clicked the button, false otherwise
*/
bool (mouse_clicked_button)(Sprite *button);

#endif
