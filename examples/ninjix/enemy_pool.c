/**
 * @file enemy_pool.c
 * @brief Enemy pool implementation -- spawn with generation handles and free-list management.
 * @ingroup group_enemies
 */

#include "enemy_pool.h"

#include <string.h>

void enemy_pool_init(enemy_pool_t *pool)
{
  if (pool == NULL)
    return;

  memset(pool, 0, sizeof(*pool));
  for (uint16_t i = 0; i < MAX_ENEMIES; i++)
    pool->free_indices[i] = (uint16_t)(MAX_ENEMIES - 1 - i);
  pool->free_count = MAX_ENEMIES;
}

enemy_handle_t enemy_pool_spawn(enemy_pool_t *pool, const enemy_type_t *type,
                                int16_t start_x, int16_t start_y)
{
  if (pool == NULL || type == NULL)
    return enemy_handle_invalid();

  if (pool->free_count == 0)
    return enemy_handle_invalid();

  // get the next free slot and initialize it with the new enemy
  uint16_t i = pool->free_indices[--pool->free_count];
  enemy_slot_t *slot = &pool->slots[i];

  slot->active = true;
  slot->generation++;
  if (slot->generation == 0)
    slot->generation = 1;

  enemy_init(&slot->enemy, type, start_x, start_y);
  pool->active_indices[pool->active_count] = i;
  pool->active_count++;

  enemy_handle_t handle = {
    .index = i,
    .generation = slot->generation
  };
  return handle;
}

enemy_t *enemy_pool_get(enemy_pool_t *pool, enemy_handle_t handle)
{
  if (pool == NULL || handle.index >= MAX_ENEMIES)
    return NULL;

  enemy_slot_t *slot = &pool->slots[handle.index];
  if (!slot->active || slot->generation != handle.generation)
    return NULL;

  return &slot->enemy;
}

const enemy_t *enemy_pool_get_const(const enemy_pool_t *pool, enemy_handle_t handle)
{
  if (pool == NULL || handle.index >= MAX_ENEMIES)
    return NULL;

  const enemy_slot_t *slot = &pool->slots[handle.index];
  if (!slot->active || slot->generation != handle.generation)
    return NULL;

  return &slot->enemy;
}

void enemy_pool_deactivate_inactive(enemy_pool_t *pool)
{
  if (pool == NULL)
    return;

  uint16_t cursor = 0;
  while (cursor < pool->active_count)
  {
    uint16_t slot_index = pool->active_indices[cursor];
    enemy_slot_t *slot = &pool->slots[slot_index];
    if (slot->active && slot->enemy.alive)
    {
      cursor++;
      continue;
    }

    // we found an inactive enemy, deactivate the slot and move the last active slot to its place in the active indices array
    slot->active = false;
    if (pool->free_count < MAX_ENEMIES)
      pool->free_indices[pool->free_count++] = slot_index;

    pool->active_count--;
    pool->active_indices[cursor] = pool->active_indices[pool->active_count];
  }
}

bool enemy_pool_empty(const enemy_pool_t *pool)
{
  return pool == NULL || pool->active_count == 0;
}
