#ifndef GRENADE_H
#define GRENADE_H

#include <stdint.h>
#include <stdbool.h>
#include "../../projectile/projectile.h"

/**
 * @brief Represents a grenade weapon projectile.
 */
typedef struct Grenade Grenade;

/**
 * @brief Creates a grenade aiming state or firing state.
 * @param x Initial X coordinate.
 * @param y Initial Y coordinate.
 * @param vx Initial X velocity.
 * @param vy Initial Y velocity.
 * @return Pointer to the newly created Grenade, or NULL on failure.
 */
Grenade* grenade_create(int32_t x, int32_t y, double vx, double vy);

/**
 * @brief Destroys a grenade projectile and frees its resources.
 * @param grenade Pointer to the Grenade to destroy.
 */
void grenade_destroy(Grenade *grenade);

/**
 * @brief Updates the grenade projectile's position and velocity.
 * @param grenade Pointer to the Grenade to update.
 */
void grenade_update(Grenade *grenade);

/**
 * @brief Retrieves the underlying generic Projectile.
 * @param grenade Pointer to the Grenade.
 * @return Pointer to the underlying Projectile.
 */
Projectile* grenade_get_projectile(const Grenade *grenade);

/**
 * @brief Checks if the grenade projectile is still active (flying).
 * @param grenade Pointer to the Grenade.
 * @return true if active, false otherwise.
 */
bool grenade_is_active(const Grenade *grenade);

/**
 * @brief Deactivates a grenade.
 * @param grenade Grenade to modify.
 */
void grenade_deactivate(Grenade *grenade);

/**
 * @brief Increments the grenade's internal tick counter.
 * @param grenade Grenade to modify.
 */
void grenade_advance_ticks(Grenade *grenade);

/**
 * @brief Gets the number of ticks the grenade has been active.
 * @param grenade Pointer to the Grenade.
 * @return Number of ticks.
 */
uint32_t grenade_get_ticks(const Grenade *grenade);

struct GameContext;

/**
 * @brief Checks collision for the grenade projectile against terrain, water and worms.
 * @param grenade Pointer to the Grenade.
 * @param ctx Pointer to the GameContext.
 * @param out_cx Pointer to store the collision X coordinate.
 * @param out_cy Pointer to store the collision Y coordinate.
 * @return true if collision detected, false otherwise.
 */
bool grenade_check_collision(const Grenade *grenade, struct GameContext *ctx, int *out_cx, int *out_cy);

#endif /* GRENADE_H */
