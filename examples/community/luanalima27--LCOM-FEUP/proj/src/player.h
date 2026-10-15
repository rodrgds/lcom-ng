/**
 * @file player.h
 * @brief Header file for player management in the game.
 */

#ifndef PLAYER_H
#define PLAYER_H

#include <lcom/lcf.h>

/**
 * @brief External variables for player position.
 */
extern int player_x;
extern int player_y;


/**
 * @brief Draws the player spaceship at the specified position.
 * @param x X position to draw the player.
 * @param y Y position to draw the player.
 * @return 0 upon success, non-zero otherwise.
 */
int (draw_player)(uint16_t x, uint16_t y);

/**
 * @brief Deletes the player spaceship from the specified position.
 * @param x X position to delete the player.
 * @param y Y position to delete the player.
 * @return 0 upon success, non-zero otherwise.
 */
int (delete_player)(uint16_t x, uint16_t y);

#endif
