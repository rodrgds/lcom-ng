/**
 * @file game_logic.h
 * @author Bernardo Santos (bernardo.ots@fe.up.pt)
 * @brief Game loop and game logic handling
 * @version 0.1
 * @date 2021-01-05
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#ifndef __GAME_LOGIC_H
#define __GAME_LOGIC_H

#include <lcom/lcf.h>

#include "constants.h"

/** @defgroup game_logic Game Logic
 * @{
 *
 * Game loop and game logic handling
 */

/**
 * @brief Game loop, where a full game's logic is handled.
 * 
 */
void play();

/**
 * @brief Handle the keyboard input, making the connection with player movement.
 * 
 * @param code makecode or breakcode of the pressed key
 */
void handle_kbd(uint8_t code);

/**
 * @brief Get the winner of the game (either RED or BLUE).
 * 
 * @return player that won the game, as winner object 
 */
winner get_winner();

/**
 * @brief Check if a goal was scored.
 * 
 * @return true if the ball passed one of the goal's line
 * @return false otherwise
 */
bool goal_scored();

/** @} end of game_logic */

#endif
