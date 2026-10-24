/**
 * @file explosion.h
 * @brief Explosion visual effects.
 */
#ifndef EXPLOSION_H
#define EXPLOSION_H

#include "game.h"
#include "../graphics/sprite.h"

/**
 * @brief Spawns a new explosion.
 * @param game Pointer to the Game state.
 * @param center_x X coordinate.
 * @param center_y Y coordinate.
 */
void explosion_spawn(Game *game, int center_x, int center_y);

/**
 * @brief Updates all active explosions.
 * @param game Pointer to the Game state.
 */
void explosion_update_all(Game *game);

/**
 * @brief Draws all active explosions.
 * @param game Pointer to the Game state.
 * @param sprite Pointer to the explosion Sprite.
 */
void explosion_draw_all(const Game *game, const Sprite *sprite);

#endif
