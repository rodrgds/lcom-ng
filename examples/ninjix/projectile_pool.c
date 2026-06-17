/**
 * @file projectile_pool.c
 * @brief Projectile pool implementation -- spawn, deactivation, and free-list management.
 * @ingroup group_projectiles
 */

#include "projectile_pool.h"

#include <string.h>

void projectile_pool_init(projectile_pool_t *pool)
{
  if (pool == NULL)
    return;

  memset(pool, 0, sizeof(*pool));
  for (uint16_t i = 0; i < MAX_PROJECTILES; i++)
    pool->free_indices[i] = (uint16_t)(MAX_PROJECTILES - 1 - i);
  pool->free_count = MAX_PROJECTILES;
}

int projectile_pool_spawn(projectile_pool_t *pool, projectile_kind_t kind,
                          enemy_handle_t target, int16_t x, int16_t y,
                          uint16_t speed, uint16_t damage,
                          uint8_t effect_type, uint16_t effect_duration, uint8_t effect_strength)
{
  if (pool == NULL || kind >= PROJECTILE_KIND_COUNT || !enemy_handle_is_valid(target))
    return 1;

  if (pool->free_count == 0)
    return 1;

  uint16_t i = pool->free_indices[--pool->free_count];
  projectile_t *projectile = &pool->projectiles[i];

  projectile->active = true;
  projectile->kind = kind;
  projectile->target = target;
  projectile->x = x;
  projectile->y = y;
  projectile->speed = speed;
  projectile->damage = damage;
  projectile->effect_type = effect_type;
  projectile->effect_duration = effect_duration;
  projectile->effect_strength = effect_strength;
  pool->active_indices[pool->active_count] = i;
  pool->active_positions[i] = pool->active_count;
  pool->active_count++;
  return 0;
}

void projectile_pool_deactivate(projectile_pool_t *pool, uint16_t index)
{
  if (pool == NULL || index >= MAX_PROJECTILES || !pool->projectiles[index].active)
    return;

  pool->projectiles[index].active = false;

  uint16_t position = pool->active_positions[index];
  if (position >= pool->active_count || pool->active_indices[position] != index)
  {
    for (position = 0; position < pool->active_count; position++)
    {
      if (pool->active_indices[position] == index)
        break;
    }

    if (position >= pool->active_count)
      return;
  }

  pool->active_count--;
  uint16_t moved_index = pool->active_indices[pool->active_count];
  pool->active_indices[position] = moved_index;
  pool->active_positions[moved_index] = position;

  if (pool->free_count < MAX_PROJECTILES)
    pool->free_indices[pool->free_count++] = index;
}

bool projectile_pool_empty(const projectile_pool_t *pool)
{
  return pool == NULL || pool->active_count == 0;
}
