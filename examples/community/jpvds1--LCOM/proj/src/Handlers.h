/**

@file Handlers.h
@brief Header file containing declarations of functions used to handle all aspects of the game.

*/

#ifndef HANDLERS_H
#define HANDLERS_H

#include <lcom/lcf.h>
#include <stdio.h>
#include <stdlib.h>
#include "proj.h"


/**
 * @brief Handles all lasers' animation, movement and collisions
 * 
 * @return int 0 upon success, non-zero otherwise.
 */
int handle_laser();
/**
 * @brief Draws the user lives, the real time clock, and the score, located on the top of the screen
 * 
 * @return int 0 upon success, non-zero otherwise.
 */
int (handle_top_bar)();
/**
 * @brief Handles enemy movements, animation and laser shooting
 * 
 * @return int 0 upon success, non-zero otherwise.
 */
int handle_enemy();
/**
 * @brief Handles user movement
 * 
 * @return int 0 upon success, non-zero otherwise.
 */
int handle_user();
/**
 * @brief Handles enemy explosion animation
 * 
 * @return int 0 upon success, non-zero otherwise.
 */
int handle_animation();
/**
 * @brief Draws laser from either the user or an enemy
 * @param x x position for the laser
 * @param y y position for the laser
 * @param user set to 1 if the laser comes from the user, 0 if it comes from an enemy
 * @return int 0 upon success, non-zero otherwise.
 */
int (draw_laser)(uint16_t x, uint16_t y, int user);
/**
 * @brief Draws the time and date on the top of the screen
 * @return int 0 upon success, non-zero otherwise.
 */
int (draw_time)();
/**
 * @brief Draws a number
 * @param number number to be drawn
 * @param x x position of the number
 * @param y y position of the number
 * @return int 0 upon success, non-zero otherwise.
 */
int (draw_number)(uint8_t number, uint16_t x, uint16_t y);
/**
 * @brief Draws user's score on the top of the screen
 * @return int 0 upon success, non-zero otherwise.
 */
int (draw_score)();

#endif //HANDLERS_H
