/**
 * @file game_viewer.h
 * @brief This file contains the definition of the game viewer module.
 * 
 * This includes the definition of the game viewer functions
 *  that display all the game information: the board, the pieces, the moves and timers.
 */

#ifndef _GAME_VIEWER_H_
#define _GAME_VIEWER_H_

#include "viewer.h"

/**
 * @brief Sets up the screenshot for the game
 * 
 * This function loads the screenshot, which will serve as background.
 * 
 * @return Returns 0 upon success, non-zero otherwise.
 */
int (game_screenshot)();

/** 
 * @brief Refreshes the game screen.
 * 
 * This function refreshes the game screen, displaying the mouse, selected piece and timers. 
 * 
 * @return Returns 0 upon success, non-zero otherwise.
*/
int (game_refresh)();

/**
 * @brief Draws the board.
 * 
 * This function draws the board on the screen.
 * 
 * @return Returns 0 upon success, non-zero otherwise.
 */
int (draw_board)();

/**
 * @brief Draws a piece.
 * 
 * This function draws a piece on the screen.
 * 
 * @param sprite the sprite of the piece.
 * @param x the x coordinate of the piece.
 * @param y the y coordinate of the piece.
 * 
 * @return Returns 0 upon success, non-zero otherwise.
 */
int (draw_piece)(Sprite* sprite, uint16_t x, uint16_t y);

/**
 * @brief Draws all the pieces.
 * 
 * This function draws all the pieces on the screen.
 * 
 * @return Returns 0 upon success, non-zero otherwise.
 */
int (draw_pieces)();

/** 
 * @brief Draws a timer.
 * 
 * This function draws a player timer on the screen.
 * 
 * @param timer the timer information to be drawn.
 * @param x the x coordinate of the timer.
 * @param y the y coordinate of the timer.
*/
int (draw_timer)(char timer[], int x, int y);

/**
 * @brief Draws both players timers.
 * 
 * @return Returns 0 upon success, non-zero otherwise.
 */
int (draw_timers)();

/**
 * @brief Erases a timer.
 * 
 * This function erases a player timer from the screen.
 * 
 * @param timer the timer information to be erased.
 * @param x the x coordinate of the timer.
 * @param y the y coordinate of the timer.
 * 
 * @return Returns 0 upon success, non-zero otherwise.
*/
int (erase_timers)();

#endif
