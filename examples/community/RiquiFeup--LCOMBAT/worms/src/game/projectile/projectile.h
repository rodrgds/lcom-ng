#ifndef PROJECTILE_H
#define PROJECTILE_H

#include <stdbool.h>
#include <stdint.h>

#include "../../engine/physics/hitbox.h"

typedef struct Projectile Projectile;

/**
 * @brief Creates an active projectile.
 * @return Pointer to the projectile, or NULL on allocation failure.
 */
Projectile* projectile_create(double x, double y,
                              double vx, double vy,
                              uint16_t width, uint16_t height,
                              uint16_t damage);

/**
 * @brief Frees a projectile.
 * @param projectile Projectile to destroy. NULL is accepted.
 */
void projectile_destroy(Projectile *projectile);

/**
 * @brief Advances a projectile by one tick and applies gravity.
 * @param projectile Projectile to update. NULL is accepted.
 */
void projectile_update(Projectile *projectile, uint16_t screen_width, uint16_t screen_height);

/**
 * @brief Gets the projectile collision hitbox.
 * @return Current hitbox, or an empty hitbox if inactive/off-screen.
 */
Hitbox projectile_get_hitbox(const Projectile *projectile);

/**
 * @brief Checks whether a projectile is active.
 * @return true if active, false otherwise.
 */
bool projectile_is_active(const Projectile *projectile);

/**
 * @brief Marks a projectile as inactive.
 * @param projectile Projectile to deactivate. NULL is accepted.
 */
void projectile_deactivate(Projectile *projectile);

/**
 * @brief Gets the projectile damage value.
 * @return Projectile damage, or 0 if projectile is NULL.
 */
uint16_t projectile_get_damage(const Projectile *projectile);

/**
 * @brief Gets the projectile x velocity.
 * @return Projectile vx, or 0 if projectile is NULL.
 */
double projectile_get_vx(const Projectile *projectile);

/**
 * @brief Gets the projectile y velocity.
 * @return Projectile vy, or 0 if projectile is NULL.
 */
double projectile_get_vy(const Projectile *projectile);

/**
 * @brief Sets the projectile x velocity.
 * @param projectile Projectile to modify.
 * @param vx New x velocity.
 */
void projectile_set_vx(Projectile *projectile, double vx);

/**
 * @brief Sets the projectile y velocity.
 * @param projectile Projectile to modify.
 * @param vy New y velocity.
 */
void projectile_set_vy(Projectile *projectile, double vy);

/**
 * @brief Gets the precise x coordinate of the projectile.
 * @param projectile Projectile to inspect.
 * @return The x coordinate.
 */
double projectile_get_precise_x(const Projectile *projectile);

/**
 * @brief Gets the precise y coordinate of the projectile.
 * @param projectile Projectile to inspect.
 * @return The y coordinate.
 */
double projectile_get_precise_y(const Projectile *projectile);

/**
 * @brief Sets the precise x coordinate of the projectile.
 * @param projectile Projectile to modify.
 * @param x The new x coordinate.
 */
void projectile_set_x(Projectile *projectile, double x);

/**
 * @brief Sets the precise y coordinate of the projectile.
 * @param projectile Projectile to modify.
 * @param y The new y coordinate.
 */
void projectile_set_y(Projectile *projectile, double y);

#endif /* PROJECTILE_H */
