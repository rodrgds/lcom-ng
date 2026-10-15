/**
 * @file name_viewer
 * @brief This file has the implementation of the functions related to the player name viewer.
 */

#ifndef _NAME_VIEWER_H_
#define _NAME_VIEWER_H_

/**
 * @brief Function to draw the name input view.
 * 
 * This function erases the cursor, draws a screenshot to a buffer, 
 * then draws the entered word and the cursor again, and finally renders the screen.
 * 
 * @return Returns 0 on success, 1 otherwise.
 */
int (name_refresh)();

/**
 * @brief Function to take a screenshot of the name input view.
 * 
 * This function cleans the screen, draws the background and the words "PLAYER" and "NAME",
 * and then copies the buffer to a screenshot.
 * 
 * @return Returns 0 on success, 1 otherwise.
 */
int (name_screenshot)();

#endif 
