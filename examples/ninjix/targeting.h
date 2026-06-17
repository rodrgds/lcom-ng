/**
 * @file targeting.h
 * @brief Tower targeting -- selects the best enemy according to targeting mode.
 * @ingroup group_towers
 *
 * Searches the enemy pool for a valid target based on the tower's
 * target_mode_t (first, last, closest, strongest, weakest, reward).
 * Returns an enemy handle or an invalid handle if no target is found.
 */

#ifndef PROJ_TARGETING_H
#define PROJ_TARGETING_H

#include "enemy_pool.h"
#include "tower.h"

/**
 * @brief Find the best enemy target for a tower based on its targeting mode.
 *
 * Searches all active enemies in the pool and returns the one that best
 * matches the tower's target_mode_t (FIRST, LAST, CLOSEST, STRONGEST,
 * WEAKEST, REWARD).  Returns an invalid handle if no target is in range.
 *
 * @param pool   Enemy pool to search.
 * @param tower  Tower seeking a target (uses position, range, target_mode).
 * @return Handle to the best target, or an invalid handle if none found.
 */
enemy_handle_t targeting_find_enemy(const enemy_pool_t *pool, const tower_t *tower);

#endif
