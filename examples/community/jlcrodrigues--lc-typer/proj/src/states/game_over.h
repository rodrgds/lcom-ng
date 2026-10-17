#ifndef GAME_OVER_H
#define GAME_OVER_H

#include "game.h"
#include "driver.h"
#include "button.h"
#include "proj.h"
#include "scoreboard.h"

/** @defgroup state game_over
 * @{
 *
 * Functions related to the game over state.
 * The game over state operates over a game struct (last played).
 */

#define TEXT_Y_POS 150

void game_over_init(void);

/**
 * @brief Draw the game over state
 * 
 * @param game Pointer to the game struct.
 */
void game_over_draw(Game* game);

/**
 * @brief Handle the latest event. May change the program's state.
 * 
 * @param game Game struct passed by reference.
 * @param event Latest event that occurred.
 */
void game_over_handle_event(Game* game, Event event);

/**
 * @brief Draw the state and handle the latest event.
 * This function calls game_over_draw and game_over_handle_event
 * 
 * @param game Game struct passed by reference.
 * @param event Latest event that occurred.
 */
void game_over_step(Game* game, Event event);

/**@}*/

#endif
