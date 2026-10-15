#ifndef TERRAIN_H
#define TERRAIN_H

#include <stdint.h>
#include <stdbool.h>
#include "../../engine/physics/hitbox.h"

/**
 * @brief Initializes the terrain map.
 * @return 0 on success, non-zero otherwise.
 */
int terrain_init(void);

/**
 * @brief Generates procedural terrain using the given RNG seed.
 *
 * Replaces the current terrain map in-place. Using the same seed produces the
 * same terrain.
 *
 * @param seed Seed for srand(). Must be non-zero.
 * @return 0 on success, 1 if seed is zero.
 */
int terrain_generate_procedural(uint32_t seed);

/**
 * @brief Cleans up terrain resources.
 */
void terrain_cleanup(void);

/**
 * @brief Checks if a given coordinate is solid terrain.
 * @param x X coordinate.
 * @param y Y coordinate.
 * @return true if collision occurs, false otherwise.
 */
bool terrain_collides(int x, int y);

/**
 * @brief Checks if a hitbox overlaps solid terrain.
 * @param hitbox Hitbox to test.
 * @return true if any sampled hitbox point collides with terrain, false otherwise.
 */
bool terrain_collides_hitbox(Hitbox hitbox);

/**
 * @brief Creates an explosion hole in the terrain.
 * @param cx X coordinate of the explosion center.
 * @param cy Y coordinate of the explosion center.
 * @param radius Radius of the explosion in pixels.
 */
void terrain_explode(int cx, int cy, int radius);

/**
 * @brief Returns a read-only pointer to the terrain collision map.
 *
 * The map is a 2D array stored as [TERRAIN_WIDTH][TERRAIN_HEIGHT].
 * A value of 1 means solid, 0 means empty (air/hole).
 *
 * @return Pointer to the terrain map data.
 */
const uint8_t *terrain_get_map(void);

#endif /* TERRAIN_H */
