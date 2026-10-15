#ifndef WATER_CONTROLLER_H
#define WATER_CONTROLLER_H

#include "water.h"
#include "../game.h"

/**
 * @brief Handles the physics and interactions of the water.
 * 
 * Instantly kills any worm that touches or falls below the water level.
 * Deactivates any active weapon projectile that falls below the water level.
 * 
 * @param w Pointer to the water model.
 * @param ctx Pointer to the game context.
 */
void water_controller_tick(const Water *w, GameContext *ctx);

#endif /* WATER_CONTROLLER_H */
