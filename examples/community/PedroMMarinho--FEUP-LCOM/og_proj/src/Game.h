#pragma once
#include <lcom/lcf.h>
#include "resources.h"

/**
 * @brief Main game loop.
 *
 * This function represents the main game loop which handles the different states of the game,
 * such as MENU and PLAYING, and processes input from the I/O devices. 
 * 
 * @param resources Pointer to the resources structure containing the current game state and other game data.
 * @return 0 on success, 1 on failure.
 */

int gameLoop(Resources* resources);
/**
 * @brief Initializes the game
 * 
 * @return int 0 if running, 1 otherwise 
*/
int initGame();
