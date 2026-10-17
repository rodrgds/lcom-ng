#pragma once

#include <lcom/lcf.h>
#include "errors.h"

#include "events/events.h"

#include "game/model/local_player.h"

void (local_player_start)();
void (local_player_end)();

/**
 * @brief Writes a character to the board
 * 
 */
void (local_player_write_char)(char c);

/**
 * @brief Removes a character from the board
 * 
 */
void (local_player_remove_char)();

/**
 * @brief Selects cell
 * 
 * @param x x coordinate
 * @param y y coordinate
 */
void (local_player_select_cell)(uint8_t x, uint8_t y);

/**
 * @brief Verifies the player's guess and returns the according status
 * 
 */
local_player_submission_status (local_player_submit_word)(bool (*verifier)(char*, cell_background*));

/**
 * @brief Handles a keypress
 * 
 * @param ev Keypress event
 */
void (local_player_handle_keypress)(struct event *ev);


/**
 * @brief Handles a mouse click
 * 
 * @param ev Mouse click event
 */
void (local_player_handle_mouse_click)(struct event *ev);
