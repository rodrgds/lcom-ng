/**
 * @file game_over_viewer
 * @brief This file has the implementation of the functions related to the game over menu viewer.
 */

#ifndef _GAME_OVER_VIEWER_H_
#define _GAME_OVER_VIEWER_H_

/**
 * @brief Draws the title of the game over screen.
 * 
 * The function loops over the words in the title and draws them on the screen. 
 * The words are drawn in a column with an offset of 100 units in the y direction for each word.
 * 
 * @return Returns 0 on success, 1 otherwise.
 */
int (draw_game_over_title)();

/**
 * @brief Draws the buttons on the game over screen.
 * 
 * This function draws the play and exit buttons on the game over screen.
 * 
 * @return Returns 0 on success, 1 otherwise.
 */
int (draw_game_over_buttons)();

/**
 * @brief Draws the complete game over screen.
 * 
 * This function draws the title and buttons of the game over screen.
 * 
 * @return Returns 0 on success, 1 otherwise.
 */
int (draw_game_over)();

/**
 * @brief Takes a screenshot of the game over screen.
 * 
 * This function cleans the screen, draws the game over screen, 
 * and copies the buffer to a screenshot.
 * 
 * @return Returns 0 on success, 1 otherwise.
 */
int (game_over_screenshot)();

/**
 * @brief Refreshes the game over screen.
 * 
 * This function erases the cursor, draws the cursor, 
 * and finally renders the screen.
 * 
 * @return Returns 0 on success, 1 otherwise.
 */
int (game_over_refresh)();


#endif
