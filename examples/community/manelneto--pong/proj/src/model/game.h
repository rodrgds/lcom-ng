#ifndef _GAME_H_
#define _GAME_H_

#include <stdbool.h>
#include <stdint.h>

/** @defgroup game game
 * @{
 *
 * Functions related to the game
 */

/**
 * @brief Starts the game
 *
 * Constructs the ball, the wall and the numbers. Initializes global variables (x_max, y_max, score and multiplier).
 *
 * @param xResolution horizontal resolution of the screen in pixels
 * @param yResolution vertical resolution of the screen in pixels
 * @param difficulty difficulty of the game
 * @return Return 0 upon success and non-zero otherwise
 */
int start_game(uint16_t xResolution, uint16_t yResolution, uint8_t difficulty);

/**
 * @brief Handles an interrupt from the timer when in the game state
 *
 * Moves the ball and updates the score.
 * If ten seconds elapsed, increases ball velocity.
 *
 */
void timer_game_handler();

/**
 * @brief Handles an interrupt from the keyboard when in the game state
 *
 * Moves the wall.
 */
void keyboard_game_handler();

/**
 * @brief Handles an interrupt from the mouse when in the game state
 *
 * Updates ball velocity and multiplier.
 */
void mouse_game_handler();

/**
 * @brief Checks for game over (the ball is out of the screen limits/bounds)
 *
 * @return Return true if game over, false otherwise
 */
bool check_game_over();

/**
 * @brief Ends the game
 *
 * Destroys the ball, the wall and the numbers.
 */
void end_game();

/**@}*/

#endif /* _GAME_H_ */
