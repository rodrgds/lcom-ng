#ifndef __PROJ_MOVEMENT_H
#define __PROJ_MOVEMENT_H

#include "model/game/game.h"

/** @defgroup movement movement
 * @{
 *
 * Functions for handling player and enemy movement
 */

/**
 * @brief Moves the player entity in the specified direction
 *
 * Handles player movement with collision detection, smooth interpolated movement,
 * and interaction with game elements (powerups, walls, enemies, etc.).
 * Updates player direction and animation based on movement.
 *
 * @param p Pointer to the player entity
 * @param game Pointer to the current game state
 * @param xmov Horizontal movement (-1, 0, or 1)
 * @param ymov Vertical movement (-1, 0, or 1)
 */
void(move_player)(Entity *p, Game *game, int16_t xmov, int16_t ymov);

/**
 * @brief Schedules movement for all active enemies
 *
 * Iterates through all enemies and initiates movement for those that are not currently moving.
 * Each enemy randomly selects a valid direction based on available empty spaces, powerups, or player position.
 * Sets up smooth movement animation and updates
 * the board state. Enemies cannot move through walls, bricks, or bombs.
 *
 * @param game Pointer to the current game state
 */
void(schedule_enemy_moves)(Game *game);

/**@}*/

#endif /* __PROJ_MOVEMENT_H */
