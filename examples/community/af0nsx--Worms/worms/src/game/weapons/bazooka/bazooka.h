#ifndef BAZOOKA_H
#define BAZOOKA_H

#include <stdint.h>
#include <stdbool.h>
#include "../../projectile/projectile.h"

/**
 * @brief Represents a bazooka weapon projectile.
 */
typedef struct Bazooka Bazooka;

/**
 * @brief Creates a bazooka aiming state or firing state.
 * @param x Initial X coordinate.
 * @param y Initial Y coordinate.
 * @param vx Initial X velocity.
 * @param vy Initial Y velocity.
 * @return Pointer to the newly created Bazooka, or NULL on failure.
 */
Bazooka* bazooka_create(double x, double y, double vx, double vy);

/**
 * @brief Destroys a bazooka projectile and frees its resources.
 * @param bazooka Pointer to the Bazooka to destroy.
 */
void bazooka_destroy(Bazooka *bazooka);

/**
 * @brief Updates the bazooka projectile's position and velocity.
 * @param bazooka Pointer to the Bazooka to update.
 */
void bazooka_update(Bazooka *bazooka);

/**
 * @brief Retrieves the underlying generic Projectile.
 * @param bazooka Pointer to the Bazooka.
 * @return Pointer to the underlying Projectile.
 */
Projectile* bazooka_get_projectile(const Bazooka *bazooka);

/**
 * @brief Checks if the bazooka projectile is still active (flying).
 * @param bazooka Pointer to the Bazooka.
 * @return true if active, false otherwise.
 */
bool bazooka_is_active(const Bazooka *bazooka);

/**
 * @brief Deactivates the bazooka projectile.
 * @param bazooka Pointer to the Bazooka to deactivate.
 */
void bazooka_deactivate(Bazooka *bazooka);

struct GameContext;

/**
 * @brief Checks collision for the bazooka projectile against terrain, water and worms.
 * @param bazooka Pointer to the Bazooka.
 * @param ctx Pointer to the GameContext.
 * @param out_cx Pointer to store the collision X coordinate.
 * @param out_cy Pointer to store the collision Y coordinate.
 * @return true if collision detected, false otherwise.
 */
bool bazooka_check_collision(const Bazooka *bazooka, struct GameContext *ctx, int *out_cx, int *out_cy);

#endif /* BAZOOKA_H */
