#ifndef GAME_H
#define GAME_H

#include <lcom/lcf.h>

#include "data_types.h"
#include "menu.h"
#include "chess.h"
#include "clock.h"
#include "../devices/dispatcher.h"
#include "../sprites/sprite.h"

/** @defgroup game game
 *  @brief Functions that create and run the game loop
*/

extern Sprite *background;

GameState state; /*!< State of the game*/ 

/**
 * @brief Initializes the game, initializing the video card, loading the sprites and setting the state to MENU
 * @return 0 upon success, 1 otherwise
 */
int game_init();

/**
 * @brief Runs the game, initializing it, running the dispatcher
 * @return 0 upon success, 1 otherwise
*/
int game_run();

/**
 * @brief Exits the game, exiting the video card and dumping the sprites
 * @return 0 upon success, 1 otherwise
*/
int game_exit();

/**
 * @brief Draws the background sprite
 * @return 0 upon success, 1 otherwise
*/
int draw_background();

#endif
