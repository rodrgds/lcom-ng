/**
 * @file enemy_pool.h
 * @brief Enemy object pool with generation-based handles.
 * @ingroup group_enemies
 *
 * The pool preallocates MAX_ENEMIES slots and reuses them via a free list.
 * Each spawned enemy receives a generation-based handle (enemy_handle_t)
 * that prevents dangling-pointer bugs after deallocation.
 */

#ifndef PROJ_ENEMY_POOL_H
#define PROJ_ENEMY_POOL_H

#include <stdbool.h>
#include <stdint.h>

#include "enemy.h"
#include "enemy_handle.h"

#define MAX_ENEMIES 128

typedef struct {
  bool active;
  uint16_t generation;
  enemy_t enemy;
} enemy_slot_t;

typedef struct {
  enemy_slot_t slots[MAX_ENEMIES];
  uint16_t active_indices[MAX_ENEMIES];
  uint16_t free_indices[MAX_ENEMIES];
  uint16_t active_count;
  uint16_t free_count;
} enemy_pool_t;

/**
 * @brief Reset the pool, marking all slots as free.
 * @param pool  Pool to initialise.
 */
void enemy_pool_init(enemy_pool_t *pool);

/**
 * @brief Spawn an enemy of the given type at the specified position.
 * @param pool     Pool to spawn into.
 * @param type     Enemy type definition (HP, speed, reward, abilities).
 * @param start_x  World X coordinate of the spawn point.
 * @param start_y  World Y coordinate of the spawn point.
 * @return Handle to the spawned enemy (valid even after subsequent spawns).
 */
enemy_handle_t enemy_pool_spawn(enemy_pool_t *pool, const enemy_type_t *type,
                                int16_t start_x, int16_t start_y);

/**
 * @brief Retrieve a mutable pointer to a live enemy by handle.
 * @param pool    Pool containing the enemy.
 * @param handle  Handle returned by enemy_pool_spawn().
 * @return Pointer to the enemy, or NULL if the handle is stale.
 */
enemy_t *enemy_pool_get(enemy_pool_t *pool, enemy_handle_t handle);

/**
 * @brief Retrieve a const pointer to a live enemy by handle.
 * @param pool    Pool containing the enemy.
 * @param handle  Handle returned by enemy_pool_spawn().
 * @return Const pointer to the enemy, or NULL if the handle is stale.
 */
const enemy_t *enemy_pool_get_const(const enemy_pool_t *pool, enemy_handle_t handle);

/**
 * @brief Deactivate all dead enemies and return their slots to the free list.
 * @param pool  Pool to sweep.
 */
void enemy_pool_deactivate_inactive(enemy_pool_t *pool);

/**
 * @brief Check whether the pool has zero active enemies.
 * @param pool  Pool to query.
 * @return true if no enemies are active.
 */
bool enemy_pool_empty(const enemy_pool_t *pool);

#endif
