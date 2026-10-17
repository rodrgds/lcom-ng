/**
 * @file projectile.h
 * @brief Logic and rendering for peashooter projectiles.
 */

#ifndef __PROJECTILE_H
#define __PROJECTILE_H

#include <stdint.h>
#include <stdbool.h>

/**
 * @brief Initializes the projectiles array and loads sprite.
 */
void projectiles_init();

/**
 * @brief Spawns a projectile in a given lane and coordinate.
 * * @param lane The lane (row) where the projectile is being fired.
 * @param x The starting x-coordinate.
 * @param y The starting y-coordinate.
 */
void spawn_projectile(uint8_t lane, int16_t x, int16_t y);

/**
 * @brief Updates the position and collision state of all active projectiles.
 */
void projectiles_update();

/**
 * @brief Renders all active projectiles.
 */
void projectiles_render();

#endif
