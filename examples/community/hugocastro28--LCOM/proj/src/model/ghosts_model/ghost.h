#ifndef GHOST_MODEL_H
#define GHOST_MODEL_H

#include "../position.h"

/**
 * @brief Game elements that move along the maze and that can kill the pacman when
 * a collision between them occurs.
 */
typedef struct {
    Position pos; 
} Ghost;

/**
 * @brief Set the initial position of the blue ghost.
 * 
 */
void (set_blue_position)();

/**
 * @brief Set the initial position of the orange ghost.
 * 
 */
void (set_orange_position)();

/**
 * @brief Set the initial position of the pink ghost.
 * 
 */
void (set_pink_position)();

/**
 * @brief Set the initial position of the red ghost.
 * 
 */
void (set_red_position)();

/**
 * @brief Set the initial position of all ghosts.
 * 
 */
void (set_ghosts_positions)();

#endif
