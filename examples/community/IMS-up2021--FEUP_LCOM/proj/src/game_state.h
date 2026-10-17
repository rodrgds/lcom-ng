#ifndef GAME_STATE_H
#define GAME_STATE_H

#include "utils.h"
#include "entity.h"
#include "draw.h"

/**
 * @brief Represents the different states of the game.
 */
typedef enum {
  PLAYING,       /**< The game is currently being played */
  MAIN_MENU,     /**< The game is in the main menu */
  INSTRUCTIONS,  /**< The game is displaying instructions */
  EXIT           /**< The game is in the process of exiting */
} game_state;

/**
 * @brief Represents a position with x and y coordinates.
 */
typedef struct Position {
  uint16_t x;    /**< The x coordinate of the position */
  uint16_t y;    /**< The y coordinate of the position */
} Position;

/**
 * @brief The current position of the mouse.
 */
extern Position mouse_pos;

/**
 * @brief The current state of the game.
 */
extern game_state state;

extern unsigned int player1_score;
extern unsigned int player2_score;

/**
 * @brief Starts the game, transitioning from the main menu to the playing state.
 *
 * This function initiates the game, setting up necessary components and 
 * changing the game state to PLAYING.
 */
void (play)();

/**
 * @brief Displays the instructions for the game.
 *
 * This function transitions the game state to INSTRUCTIONS, where the user 
 * can read how to play the game.
 */
void (instructions)();

/**
 * @brief Updates the mouse position.
 *
 * This function updates the global mouse position based on the input x and y 
 * deltas.
 *
 * @param x The change in the x position of the mouse.
 * @param y The change in the y position of the mouse.
 */
void (update_mouse)(int16_t x, int16_t y);

/**
 * @brief Exits the game.
 *
 * This function performs necessary cleanup and transitions the game state to EXIT.
 */
void (exit_game)();

/**
 * @brief Moves the player or cursor up.
 *
 * This function handles the logic for moving the player or cursor upwards.
 */
void (move_up)(Player *player);

/**
 * @brief Moves the player or cursor down.
 *
 * This function handles the logic for moving the player or cursor downwards.
 */
void (move_down)(Player *player);


#endif
