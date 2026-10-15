/**
 * @file game_over_view.h
 * @brief Interface for rendering the game over screen and related UI elements.
 */

#ifndef _LCOM_GAME_OVER_MENU_VIEW_H_
#define _LCOM_GAME_OVER_MENU_VIEW_H_
#include <lcom/lcf.h>

#include "view/view.h"

/**
 * @brief Draws the game over screen frame.
 * 
 * Renders the game over screen, displaying the results of the game, including the winner in the
 * case of 2 players, reaction times, and options to play again or return to the menu.
 */
void draw_game_over_frame();

#endif /* _LCOM_GAME_OVER_MENU_VIEW_H_ */
