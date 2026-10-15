#ifndef WEAPON_VIEWER_UTILS_H
#define WEAPON_VIEWER_UTILS_H

#include <stdint.h>
#include "../../projectile/projectile.h"
#include "../../../engine/graphics/sprite.h"

/**
 * @brief Draws a projectile using a set of rotated sprite frames based on its velocity.
 * @param p Pointer to the projectile.
 * @param frames Array of sprite frames representing rotations.
 * @param count Number of frames in the array.
 */
void weapon_draw_rotated(const Projectile *p, Sprite *const *frames, uint8_t count);

/**
 * @brief Draws the aiming trajectory line and power bar.
 * @param wx Worm's x coordinate.
 * @param wy Worm's y coordinate.
 * @param aim_angle Current aim angle.
 * @param aim_power Current aim power.
 * @param dir Worm's direction (1 for right, -1 for left).
 * @param frames Array of angle sprite frames.
 * @param count Number of angle frames.
 * @param projectile_speed Full-power projectile speed.
 * @param projectile_size Square projectile sprite size.
 */
void weapon_draw_aim_trajectory(int wx, int wy, double aim_angle, int aim_power,
                                int dir, Sprite *const *frames, uint8_t count,
                                double projectile_speed, uint16_t projectile_size);

#endif /* WEAPON_VIEWER_UTILS_H */
