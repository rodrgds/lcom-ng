#ifndef WORM_H
#define WORM_H

#include <stdbool.h>
#include <stdint.h>

#include "../../engine/physics/hitbox.h"

typedef struct Worm Worm;

typedef enum {
  WEAPON_NONE,
  WEAPON_BAZOOKA,
  WEAPON_GRENADE
} WeaponType;

typedef enum {
  WORM_IDLE,
  WORM_WALK,
  WORM_JUMP,
  WORM_FALL,
  WORM_WEAPON_PULL,
  WORM_WEAPON_HOLD,
  WORM_WEAPON_RETRACT,
  WORM_DEAD,
  WORM_DROWNED
} WormState;

typedef enum {
  WORM_DIR_LEFT = 0,
  WORM_DIR_RIGHT = 1
} WormDirection;

/**
 * @brief Creates a living worm at a top-left position.
 * @param x Initial x coordinate.
 * @param y Initial y coordinate.
 * @return Pointer to the worm, or NULL on allocation failure.
 */
Worm *worm_create(int32_t x, int32_t y);

/**
 * @brief Frees a worm.
 * @param worm Worm to destroy. NULL is accepted.
 */
void worm_destroy(Worm *worm);

/**
 * @brief Applies damage, clamping health at zero.
 * @param worm Worm taking damage. NULL is accepted.
 * @param damage Damage amount.
 */
void worm_take_damage(Worm *worm, uint16_t damage);

/**
 * @brief Gets the current collision area.
 * @return A hitbox, or an empty hitbox for NULL, dead, or negative-position worms.
 */
Hitbox worm_get_hitbox(const Worm *worm);

/**
 * @brief Returns the body-tight hitbox used exclusively for weapon damage checks.
 *
 * Smaller than the physics hitbox returned by worm_get_hitbox(). The physics
 * hitbox must not change because physics_controller.c relies on its exact
 * dimensions for terrain collision and step-up logic.
 *
 * @param worm Worm to inspect. NULL returns an empty hitbox.
 * @return Damage hitbox, or an empty hitbox for NULL / dead / off-screen worms.
 */
Hitbox worm_get_damage_hitbox(const Worm *worm);

/**
 * @brief Gets the horizontal position.
 * @param worm Worm to inspect. Must not be NULL.
 * @return Position.
 */
int32_t worm_get_x(const Worm *worm);

/**
 * @brief Gets the vertical position.
 * @param worm Worm to inspect. Must not be NULL.
 * @return Position.
 */
int32_t worm_get_y(const Worm *worm);

/**
 * @brief Gets the horizontal velocity.
 * @param worm Worm to inspect. NULL returns 0.
 * @return Horizontal velocity in pixels per tick.
 */
double worm_get_vx(const Worm *worm);

/**
 * @brief Gets the vertical velocity.
 * @param worm Worm to inspect. NULL returns 0.
 * @return Vertical velocity in pixels per tick.
 */
double worm_get_vy(const Worm *worm);

/**
 * @brief Gets the precise horizontal position.
 * @param worm Worm to inspect. NULL returns 0.
 * @return Position in pixels.
 */
double worm_get_precise_x(const Worm *worm);

/**
 * @brief Gets the precise vertical position.
 * @param worm Worm to inspect. NULL returns 0.
 * @return Position in pixels.
 */
double worm_get_precise_y(const Worm *worm);

/**
 * @brief Gets current health.
 * @param worm Worm to inspect. Must not be NULL.
 * @return Health.
 */
uint16_t worm_get_health(const Worm *worm);

/**
 * @brief Checks whether a worm remains alive.
 * @return true when it has non-zero health, false otherwise.
 */
bool worm_is_alive(const Worm *worm);

/**
 * @brief Checks whether the worm is currently supported by ground.
 * @param worm Worm to inspect. NULL returns false.
 * @return true if grounded, false otherwise.
 */
bool worm_is_grounded(const Worm *worm);

/**
 * @brief Gets the current logical state of the worm.
 * @param worm Worm to inspect. NULL returns WORM_DEAD.
 * @return Current WormState.
 */
WormState worm_get_state(const Worm *worm);

/**
 * @brief Gets the direction the worm is facing.
 * @param worm Worm to inspect. NULL returns WORM_DIR_RIGHT.
 * @return Current WormDirection.
 */
WormDirection worm_get_direction(const Worm *worm);

/**
 * @brief Sets the logical state of the worm.
 * @param worm  Worm to update. NULL is accepted (no-op).
 * @param state New state.
 */
void worm_set_state(Worm *worm, WormState state);

/**
 * @brief Sets the direction the worm faces.
 * @param worm Worm to update. NULL is accepted (no-op).
 * @param dir  New direction.
 */
void worm_set_direction(Worm *worm, WormDirection dir);

/**
 * @brief Sets the worm top-left position.
 * @param worm Worm to update. NULL is accepted.
 * @param x New horizontal position.
 * @param y New vertical position.
 */
void worm_set_position(Worm *worm, double x, double y);

/**
 * @brief Sets the horizontal velocity.
 * @param worm Worm to update. NULL is accepted.
 * @param vx New horizontal velocity.
 */
void worm_set_vx(Worm *worm, double vx);

/**
 * @brief Sets the vertical velocity.
 * @param worm Worm to update. NULL is accepted.
 * @param vy New vertical velocity.
 */
void worm_set_vy(Worm *worm, double vy);

/**
 * @brief Sets whether the worm is grounded.
 * @param worm Worm to update. NULL is accepted.
 * @param grounded New grounded state.
 */
void worm_set_grounded(Worm *worm, bool grounded);

/**
 * @brief Moves the worm horizontally by dx pixels.
 * @param worm Worm to move. NULL or dead worms are ignored.
 * @param dx   Horizontal displacement (positive = right).
 */
void worm_move(Worm *worm, double dx);

/**
 * @brief Starts upward jump velocity for a living worm.
 * @param worm Worm to jump. NULL is accepted.
 */
void worm_jump(Worm *worm);

/**
 * @brief Gets the worm's currently equipped weapon type.
 */
WeaponType worm_get_weapon(const Worm *worm);

/**
 * @brief Sets the worm's equipped weapon type.
 */
void worm_set_weapon(Worm *worm, WeaponType weapon);

#endif /* WORM_H */
