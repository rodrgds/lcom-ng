#ifndef EXPLOSION_CONTROLLER_H
#define EXPLOSION_CONTROLLER_H

#include <stdint.h>
#include "../game.h"
#include "explosion.h"

/**
 * @brief Spawns an explosion, causing immediate terrain deformation and worm damage.
 * 
 * @param ctx Game context to access terrain and worms.
 * @param x X coordinate of the explosion.
 * @param y Y coordinate of the explosion.
 * @param radius Explosion radius for terrain destruction and damage calculation.
 * @param max_damage Damage applied at the exact center of the explosion.
 * @return Pointer to the newly created visual Explosion model, or NULL on failure.
 */
Explosion* explosion_controller_spawn(GameContext *ctx, int32_t x, int32_t y, int radius, int max_damage);

/**
 * @brief Ticks all active explosions in the game context.
 * 
 * @param ctx Game context containing the explosions.
 */
void explosion_controller_tick_all(GameContext *ctx);

#endif /* EXPLOSION_CONTROLLER_H */
