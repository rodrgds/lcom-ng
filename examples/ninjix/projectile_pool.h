/**
 * @file projectile_pool.h
 * @brief Projectile object pool -- spawn, movement, collision, and damage.
 * @ingroup group_projectiles
 *
 * Projectiles are created by towers when they fire. Each projectile moves
 * toward its target, applies damage and optional effects on impact, and is
 * deactivated when it reaches the target or goes off-screen. The pool
 * reuses deactivated slots via a free list.
 */

#ifndef PROJ_PROJECTILE_POOL_H
#define PROJ_PROJECTILE_POOL_H

#include <stdbool.h>
#include <stdint.h>

#include "enemy_handle.h"
#include "projectile_kind.h"
#include "tower.h"

#define MAX_PROJECTILES 128

typedef struct {
  bool active;
  projectile_kind_t kind;
  enemy_handle_t target;
  int16_t x;
  int16_t y;
  uint16_t speed;
  uint16_t damage;
  uint8_t effect_type;
  uint16_t effect_duration;
  uint8_t effect_strength;
} projectile_t;

typedef struct {
  projectile_t projectiles[MAX_PROJECTILES];
  uint16_t active_indices[MAX_PROJECTILES];
  uint16_t active_positions[MAX_PROJECTILES];
  uint16_t free_indices[MAX_PROJECTILES];
  uint16_t active_count;
  uint16_t free_count;
} projectile_pool_t;

/**
 * @brief Reset the projectile pool, marking all slots as free.
 * @param pool  Pool to initialise.
 */
void projectile_pool_init(projectile_pool_t *pool);

/**
 * @brief Spawn a projectile heading toward a target enemy.
 * @param pool             Pool to spawn into.
 * @param kind             Projectile visual type.
 * @param target           Handle of the target enemy.
 * @param x                Spawn X position in pixels.
 * @param y                Spawn Y position in pixels.
 * @param speed            Movement speed in pixels per tick.
 * @param damage           Damage dealt on impact.
 * @param effect_type      Status effect to apply (TOWER_EFFECT_NONE if none).
 * @param effect_duration  Effect duration in ticks.
 * @param effect_strength  Effect strength (0-255).
 * @return 0 on success, 1 if the pool is full.
 */
int projectile_pool_spawn(projectile_pool_t *pool, projectile_kind_t kind,
                          enemy_handle_t target, int16_t x, int16_t y,
                          uint16_t speed, uint16_t damage,
                          uint8_t effect_type, uint16_t effect_duration, uint8_t effect_strength);

/**
 * @brief Deactivate a projectile and return its slot to the free list.
 * @param pool   Pool containing the projectile.
 * @param index  Index of the projectile to deactivate.
 */
void projectile_pool_deactivate(projectile_pool_t *pool, uint16_t index);

/**
 * @brief Check whether the pool has zero active projectiles.
 * @param pool  Pool to query.
 * @return true if no projectiles are active.
 */
bool projectile_pool_empty(const projectile_pool_t *pool);

#endif
