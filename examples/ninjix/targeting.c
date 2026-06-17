/**
 * @file targeting.c
 * @brief Tower targeting implementation -- enemy selection by targeting mode.
 * @ingroup group_towers
 */

#include "targeting.h"

#include <limits.h>
#include <stdlib.h>

static int32_t targeting_dist_sq(int16_t ax, int16_t ay, int16_t bx, int16_t by)
{
  int32_t dx = (int32_t)ax - bx;
  int32_t dy = (int32_t)ay - by;
  return dx * dx + dy * dy;
}

static int32_t targeting_path_progress(const enemy_t *enemy)
{
  // Why the heck is this multiplied by 1000? Eventually we might want to add more precision to the path progress (like how far along the current segment the enemy is) so I guess this is a start
  return (int32_t)enemy->wp_index * 1000;
}

enemy_handle_t targeting_find_enemy(const enemy_pool_t *pool, const tower_t *tower)
{
  enemy_handle_t best = enemy_handle_invalid();

  if (pool == NULL || tower == NULL || tower->type == NULL)
    return best;

  int32_t best_score = INT_MIN;
  int32_t best_progress = INT_MIN;
  int32_t range_sq = (int32_t)tower->type->range * tower->type->range;
  int16_t tower_center_x = (int16_t)(tower->x + tower->type->frame_width / 2);
  int16_t tower_center_y = (int16_t)(tower->y + tower->type->frame_height / 2);

  for (uint16_t active_i = 0; active_i < pool->active_count; active_i++)
  {
    uint16_t i = pool->active_indices[active_i];
    const enemy_slot_t *slot = &pool->slots[i];
    if (!slot->active || !slot->enemy.alive)
      continue;

    bool tower_is_smoke = (tower->type->effect_type == TOWER_EFFECT_SLOW);
    if (!tower_is_smoke && (slot->enemy.type->abilities & ENEMY_ABILITY_INVISIBLE) && !slot->enemy.revealed)
      continue;

    int32_t dist_sq = targeting_dist_sq(tower_center_x, tower_center_y, slot->enemy.x, slot->enemy.y);
    if (dist_sq > range_sq)
      continue;

    int32_t progress = targeting_path_progress(&slot->enemy);
    int32_t score = 0;
    switch (tower->target_mode)
    {
      case TARGET_LAST:
        score = -progress;
        break;
      case TARGET_CLOSEST:
        score = -dist_sq;
        break;
      case TARGET_STRONGEST:
        score = slot->enemy.health;
        break;
      case TARGET_WEAKEST:
        score = -slot->enemy.health;
        break;
      case TARGET_REWARD:
        score = slot->enemy.type->reward;
        break;
      case TARGET_FIRST:
      default:
        score = progress;
        break;
    }

    if (score > best_score)
    {
      best_score = score;
      best_progress = progress;
      best.index = i;
      best.generation = slot->generation;
    }
    else if (score == best_score)
    {
      // tie-breaking: prefer enemies that are further along the path
      if (progress > best_progress)
      {
        best_progress = progress;
        best.index = i;
        best.generation = slot->generation;
      }
    }
  }

  return best;
}
