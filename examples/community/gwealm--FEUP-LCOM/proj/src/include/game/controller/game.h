#pragma once

#include <lcom/lcf.h>
#include "errors.h"

#include "events/events.h"

#include "game/model/game.h"
#include "game/model/cell_background.h"

/**
 * @brief Starts a game instance
 * 
 */
void (game_start)();

/**
 * @brief Finishes a game instance
 * 
 */
void (game_end)();

/**
 * @brief Goes back to the menu
 * 
 */
void (game_go_back)();

/**
 * @brief Handles a keypress
 * 
 * @param ev Keypress event
 */
void (game_handle_keypress)(struct event *ev);

/**
 * @brief Handles scheduler tick
 * 
 */
void (game_handle_scheduler_tick)();

/**
 * @brief Verifies if the guess is correct and changes backgrounds accordingly
 * 
 * @param word The user's guess
 * @param line The current line the player's on
 * @return true if the guess is the target word, false otherwise
 */
bool (game_verify_word)(char *word, cell_background *line);
