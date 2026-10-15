#ifndef _GAME_OVER_VIEWER_H_
#define _GAME_OVER_VIEWER_H_

/**
 * @file game_over_viewer.h
 * @brief Header file containing function declarations for game over menu viewer functions.
 */

#include "viewer.h"

/** @defgroup game_over_viewer game_over_viewer
 * @{
  */
extern Sprite* game_over;
extern Sprite* play_again;
extern Sprite* game_exit;

/**
 * @brief Draws the game over menu.
 * @return 1 if the drawing is successful, 0 otherwise.
 */
int (draw_game_over_menu)();
/**
 * @brief Draws the cursor in the game over menu and updates its position based on the selected item.
 * @return 1 if the drawing is successful, 0 otherwise.
 */
int (draw_game_over_cursor)();

#endif
