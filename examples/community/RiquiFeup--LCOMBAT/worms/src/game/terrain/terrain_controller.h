#ifndef TERRAIN_CONTROLLER_H
#define TERRAIN_CONTROLLER_H

#include "terrain.h"

/**
 * @brief Initializes terrain procedurally using an RTC-derived seed.
 * @return 0 on success, 1 on failure.
 */
int terrain_controller_init(void);

/**
 * @brief Processes an explosion event on the terrain.
 *
 * Delegates to terrain_explode() in the model.
 * Placeholder for future logic (e.g., chain reactions, sound triggers).
 *
 * @param cx     X coordinate of explosion center.
 * @param cy     Y coordinate of explosion center.
 * @param radius Explosion radius in pixels.
 */
void terrain_controller_handle_explosion(int cx, int cy, int radius);

#endif /* TERRAIN_CONTROLLER_H */
