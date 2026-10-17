/**
 * @file game.h
 * @brief Declares the main game states and the game loop function.
 */

#ifndef _GAME_H_
#define _GAME_H_

#include <lcom/lcf.h>

/**
 * @struct GameState
 * @brief Enum representing the different states of the game.
 */
typedef enum {
  START_MENU,
  INSTRUCTIONS,
  SHOW_ORIGINAL,
  USER_PAINT,
  RESULTS,
  EXIT_GAME
} GameState;

/**
 * @brief Main game loop. Handles state transitions and event processing.~
 * 
 * @return 0 on success, non-zero otherwise.
 */
int game_loop();

#endif // _GAME_H_
