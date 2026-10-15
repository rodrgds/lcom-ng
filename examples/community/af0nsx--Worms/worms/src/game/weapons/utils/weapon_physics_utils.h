#ifndef WEAPON_PHYSICS_UTILS_H
#define WEAPON_PHYSICS_UTILS_H

#include <stdbool.h>
#include <stdint.h>
#include "../../projectile/projectile.h"
#include "../../worm/worm.h"

struct GameContext;

/**
 * @brief Function pointer type for weapon collision checks.
 */
typedef bool (*WeaponCollisionFunc)(const void *weapon, struct GameContext *ctx, int *out_cx, int *out_cy);

/**
 * @brief Gets the projectile spawn position at a worm's facing-side muzzle.
 *
 * @param worm Pointer to the firing worm.
 * @param projectile_size Square projectile sprite size.
 * @param spawn_x Pointer to store the top-left projectile X coordinate.
 * @param spawn_y Pointer to store the top-left projectile Y coordinate.
 * @return true on success, false if any pointer is NULL.
 */
bool weapon_get_muzzle_spawn(const Worm *worm, uint16_t projectile_size, int32_t *spawn_x, int32_t *spawn_y);

/**
 * @brief Computes velocity from an aim angle and power percentage.
 *
 * @param angle Aim angle in radians.
 * @param power Power percentage.
 * @param speed Full-power projectile speed.
 * @param vx Pointer to store X velocity.
 * @param vy Pointer to store Y velocity.
 * @return true on success, false for invalid outputs or non-positive power.
 */
bool weapon_velocity_from_angle(double angle, int power, double speed, double *vx, double *vy);

/**
 * @brief Prepares an angle/power shot from a worm's facing-side muzzle.
 *
 * @param worm Pointer to the firing worm.
 * @param projectile_size Square projectile sprite size.
 * @param angle Aim angle in radians.
 * @param power Power percentage.
 * @param speed Full-power projectile speed.
 * @param start_x Pointer to store projectile start X.
 * @param start_y Pointer to store projectile start Y.
 * @param vx Pointer to store X velocity.
 * @param vy Pointer to store Y velocity.
 * @return true on success, false if spawn or velocity calculation fails.
 */
bool weapon_prepare_aimed_shot_from_worm(const Worm *worm, uint16_t projectile_size,
                                         double angle, int power, double speed,
                                         int32_t *start_x, int32_t *start_y,
                                         double *vx, double *vy);

/**
 * @brief Updates shared weapon aim controls.
 *
 * @param aim_angle Pointer to the current aim angle.
 * @param aim_power Pointer to the current aim power percentage.
 * @param space_held Pointer to the previous space-key state.
 * @return true when space was released and the weapon should fire.
 */
bool weapon_update_aim_input(double *aim_angle, int *aim_power, bool *space_held);

/**
 * @brief Updates a projectile and applies bouncing physics on collision.
 *
 * @param p Pointer to the projectile.
 * @param weapon Pointer to the weapon model (e.g. Grenade).
 * @param ctx Pointer to the GameContext.
 * @param check_collision Callback function to check for collisions.
 * @param restitution The bounciness factor (e.g. 0.6).
 * @param friction The horizontal friction factor when sliding (e.g. 0.95).
 */
void weapon_physics_bounce_update(Projectile *p, const void *weapon, struct GameContext *ctx,
                                  WeaponCollisionFunc check_collision, double restitution, double friction);

#endif /* WEAPON_PHYSICS_UTILS_H */
