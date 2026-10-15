/**
 * @file game
 * @brief Functions for the integration of all game
 */

#ifndef _LCOM_GAME_H_
#define _LCOM_GAME_H_

/** @defgroup Game Game
 * @{
 *
 * Functions for the integration of all game
 */

#include "main_game/main_game.h"
#include "menu/menu.h"
#include "record/record.h"

/**
 * @brief Runs all the game regarding each state of the game
 *
 * @return Return 0 upon success and 1 otherwise
 */
int (run_game)();

/**
 * @brief Runs the mouse actions regarding each state of the game
 *
 * @return Return 0 upon success and 1 otherwise
 */
int (game_mouse)();

/**
 * @brief Runs the keyboard actions regarding each state of the game
 *
 */
void (game_kbc)();

/**
 * @brief Increase the waiting time when the game is in waiting mode
 *
 */
void (waiting_mode_timer)();

/**@}*/

#endif
