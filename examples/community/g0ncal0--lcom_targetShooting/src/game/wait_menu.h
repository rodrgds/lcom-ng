#ifndef _WAIT_MENU_H_
#define _WAIT_MENU_H_

#include <lcom/lcf.h>

#include "sprite.h"
#include "game.h"

/**
 * @brief Draws the wait menu on the screen (multiplayer game).
 * This function is responsible for displaying the wait menu
 * screen, showing the message to wait for the other payer.
 * 
 * @param isDay boolean that indicates if the game is in day mode or night mode
 */
void draw_wait_menu(bool isDay);

#endif /* WAIT_MENU_H_ */
