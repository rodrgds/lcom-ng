#ifndef _GAMEOVER_H_
#define _GAMEOVER_H_

#include <lcom/lcf.h>

#include "game.h"
#include "sprite.h"

/**
 * DRAW FUNCTIONS
 */

/**
 * Draws the game over score on the screen.
 * This function is responsible for displaying the final score
 * achieved by the player after the game is over.
 */
void (draw_GameOverScore)();

/**
 * Draws the game over screen on the screen.
 * This function is responsible for displaying the game over
 * screen, showing the final score and the message to exit the game.
 * 
 * @param isDay boolean that indicates if the game is in day mode or night mode
 */
void (draw_gameover)(bool isDay);

#endif /* _GAMEOVER_H_ */
