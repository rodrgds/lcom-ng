#ifndef __PROJ_COMBAT_H
#define __PROJ_COMBAT_H

#include "model/game/game.h"

/** @defgroup combat combat
 * @{
 *
 * Functions for handling bombs, explosions, and combat mechanics
 */

/**< @brief Duration of explosion animation in ticks */
#define EXPLOSION_LIFETIME_TICKS (EXPLOSION_ANIM_FRAMES * EXPLOSION_ANIM_SPEED)

/**
 * @brief Drops a bomb at the specified coordinates
 *
 * Places a bomb entity at the given position if valid. The bomb will explode
 * after a set number of ticks. Validates coordinates, checks for empty space,
 * and ensures the position is adjacent to the player.
 *
 * @param game Pointer to the current game state
 * @param x X-coordinate on the game grid
 * @param y Y-coordinate on the game grid
 */
void(drop_bomb)(Game *game, int16_t x, int16_t y);

/**
 * @brief Explodes a bomb and handles damage to surrounding entities
 *
 * Detonates the specified bomb, creating explosion effects and dealing damage
 * to entities within the blast radius. Handles chain reactions, entity destruction,
 * and level progression when all bricks are destroyed.
 *
 * @param game Pointer to the current game state
 * @param bomb_index Index of the bomb to explode in the bombs array
 */
void(explode_bomb)(Game *game, uint8_t bomb_index);

/**
 * @brief Updates all active bombs and handles explosions
 *
 * Decrements bomb timers and triggers explosions when bombs reach zero.
 * Updates bomb animations and compacts the bombs array by removing inactive bombs.
 *
 * @param game Pointer to the current game state
 */
void(update_bombs)(Game *game);

/**
 * @brief Updates all active explosion animations
 *
 * Manages explosion entity lifetimes, updates their animations, and removes
 * expired explosions. Compacts the explosions array for efficiency.
 *
 * @param game Pointer to the current game state
 */
void(update_explosions)(Game *game);

/**@}*/

#endif /* __PROJ_COMBAT_H */
