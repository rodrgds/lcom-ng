/**
 * @file elements.h
 * @author Bernardo Santos (bernardo.ots@fe.up.pt)
 * @brief Game objects creation and handling
 * @version 0.1
 * @date 2021-01-05
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#ifndef __ELEMENTS_H
#define __ELEMENTS_H

/** @defgroup elements Elements
 * @{
 *
 * Game objects creation and handling
 */

#include <lcom/lcf.h>

#include "sprite.h"

/**
 * @brief Create the game movable elements (players and ball) as sprites.
 * 
 */
void create_elements();

/**
 * @brief Reset the elements' positions and speeds to their original ones.
 * 
 */
void reset_positions();

/**
 * @brief Erase the game elements from memory and release used resources.
 * 
 */
void destroy_elements();

/**
 * @brief Draw all the static (non-movable) game elements (background, goals, scoreboard and time board).
 * 
 * @return 0 on success, 1 on failure 
 */
int draw_static_elements();

/**
 * @brief Get the game elements.
 * 
 * @return the array of pointers to sprites where the game elements are stored
 */
Sprite **get_elements();

/**
 * @brief Get the game background pixmap.
 * 
 * @return background pixmap 
 */
uint8_t *get_background();

/**
 * @brief Load the pitch pixmap and draw it as the game background.
 * 
 * @return 0 on success, 1 on failure 
 */
int draw_background();

/**
 * @brief Load both goals pixmaps and draw them on the screen.
 * 
 * @return 0 on success, 1 on failure 
 */
int draw_goals();

/**
 * @brief Load the scoreboard pixmap and draw it on the screen.
 * 
 * @return 0 on success, 1 on failure 
 */
int draw_scoreboard();

/**
 * @brief Update the scores on the scoreboard and redraw it on the screen.
 * 
 * @param red goals scored by the red player
 * @param blue goals scored by the blue player
 * @return 0 on success, 1 on failure 
 */
int update_scores(int red, int blue);

/**
 * @brief Load the time board pixmap and draw it on the screen.
 * 
 * @return 0 on success, 1 on failure 
 */
int draw_time_board();

/**
 * @brief Update the time on the time board and redraw it on the screen.
 * 
 * @param min minutes digit value
 * @param sec1 seconds first digit value
 * @param sec2 seconds second digit value
 * @return 0 on success, 1 on failure 
 */
int update_time(int min, int sec1, int sec2);

/**
 * @brief Load the winner message pixmap and draw it on the screen.
 * 
 * @param win_player player that won the game
 * @return 0 on success, 1 on failure 
 */
int draw_winner_message(winner win_player);

/** @} end of elements */

#endif
