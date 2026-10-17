#ifndef COLLISIONS_CONTROLLER_H
#define COLLISIONS_CONTROLLER_H

#include "../../model/position.h"
#include "../../model/coins_model/coin.h"
#include "../../model/ghosts_model/ghost.h"
#include "../../model/pacman_model/pacman.h"

/**
 * @brief Checks if there was a collision between the pacman and any ghosts.
 * 
 */
bool (pacman_ghost_collision)();

/**
 * @brief Checks if a given ghost collides with the pacman.
 * @param ghost_pos is the position of the ghost.
 * 
 */
bool (ghost_pacman_collision)(Position ghost_pos);

/**
 * @brief Checks if a given position collides with the maze.
 * @param position is the position.
 * 
 */
bool (maze_collision)(Position position);

/**
 * @brief Checks if the pacman captured any small coin.
 * 
 */
bool (pacman_small_coin_collision)();

/**
 * @brief Checks if the pacman captured any big coin.
 * 
 */
bool (pacman_big_coin_collision)();

/**
 * @brief Checks if the pacman captured any big coin.
 * 
 */
bool (ghosts_collision)(Position newPosition, int ghostIndex);

#endif
