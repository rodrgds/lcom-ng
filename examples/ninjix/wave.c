/**
 * @file wave.c
 * @brief Wave controller implementation -- spawn scheduling and burst management.
 * @ingroup group_waves
 */

#include "wave.h"

#include <stdlib.h>

void wave_ctrl_init(wave_controller_t *ctrl, const wave_t *waves, uint16_t wave_count,
                    const enemy_type_t enemy_types[ENEMY_KIND_COUNT],
                    int16_t spawn_x, int16_t spawn_y,
                    int16_t *path_x, int16_t *path_y, uint16_t path_len)
{
  if (ctrl == NULL || waves == NULL)
    return;

  ctrl->waves = waves;
  ctrl->wave_count = wave_count;
  ctrl->current_wave = 0;
  ctrl->entry_queue = queue_init();
  ctrl->timer = (wave_count > 0) ? waves[0].pre_wave_delay : 0;
  enemy_pool_init(&ctrl->enemies);

  for (uint8_t i = 0; i < ENEMY_KIND_COUNT; i++)
    ctrl->enemy_types[i] = &enemy_types[i];

  ctrl->spawn_x = spawn_x;
  ctrl->spawn_y = spawn_y;
  ctrl->path_x = path_x;
  ctrl->path_y = path_y;
  ctrl->path_len = path_len;
  ctrl->wave_delay = 600;
  ctrl->wave_active = false;
  ctrl->level_complete = false;
}

void wave_ctrl_destroy(wave_controller_t *ctrl)
{
  if (ctrl == NULL)
    return;

  while (!queue_empty(ctrl->entry_queue))
  {
    wave_entry_t *entry = queue_dequeue(ctrl->entry_queue);
    free(entry);
  }

  queue_destroy(ctrl->entry_queue);
  ctrl->entry_queue = NULL;
}

/**
 * @brief Advance the wave controller by one physics tick.
 *
 * Manages per-wave spawn delays, entry queuing, burst scheduling, and
 * wave transitions. Each spawn creates an enemy via enemy_pool_spawn()
 * with the correct type and start position.
 *
 * @param ctrl Wave controller to step.
 * @sideeff May spawn one or more enemies into the enemy pool.
 * @callgraph
 */
void wave_ctrl_step(wave_controller_t *ctrl)
{
  if (ctrl == NULL || ctrl->level_complete)
    return;

  if (ctrl->timer > 0)
  {
    ctrl->timer--;
    return;
  }

  if (!ctrl->wave_active)
  {
    // the next wave should start now!
    const wave_t *wave = &ctrl->waves[ctrl->current_wave];

    for (uint8_t i = 0; i < wave->entry_count; i++)
    {
      wave_entry_t *entry = malloc(sizeof(wave_entry_t));
      if (entry == NULL)
        return;

      *entry = wave->entries[i];
      entry->burst_remaining = entry->burst_size;
      entry->spawned = 0;
      queue_enqueue(ctrl->entry_queue, entry);
    }

    ctrl->wave_active = true;
    ctrl->timer = 0;
    return;
  }

  if (queue_empty(ctrl->entry_queue))
  {
    if (!enemy_pool_empty(&ctrl->enemies))
      // there are still active enemies, wait for them to be cleared before starting the next wave or ending the level
      return;

    // the wave is officially complete now, move on to the next one
    ctrl->wave_active = false;
    ctrl->current_wave++;

    if (ctrl->current_wave >= ctrl->wave_count)
      ctrl->level_complete = true;
    else
      ctrl->timer = ctrl->waves[ctrl->current_wave].pre_wave_delay;

    return;
  }

  wave_entry_t *entry = queue_front(ctrl->entry_queue);

  enemy_pool_spawn(&ctrl->enemies, ctrl->enemy_types[entry->kind], ctrl->spawn_x, ctrl->spawn_y);

  entry->burst_remaining--;

  if (entry->burst_remaining == 0)
  {
    entry->spawned += entry->burst_size;

    if (entry->spawned < entry->count)
    {
      uint16_t remaining = entry->count - entry->spawned;
      entry->burst_remaining = (remaining < entry->burst_size) ? (uint8_t)remaining : entry->burst_size;
      ctrl->timer = entry->spawn_interval;
    }
    else
    {
      queue_dequeue(ctrl->entry_queue);
      free(entry);
      ctrl->timer = 0;
    }
  }
  else
  {
    ctrl->timer = entry->burst_interval;
  }
}

void wave_ctrl_skip_delay(wave_controller_t *ctrl)
{
  if (ctrl == NULL)
    return;
  ctrl->timer = 0;
}

void wave_ctrl_deactivate_inactive_enemies(wave_controller_t *ctrl)
{
  if (ctrl == NULL)
    return;

  enemy_pool_deactivate_inactive(&ctrl->enemies);
}

bool wave_ctrl_waiting_for_clear(const wave_controller_t *ctrl)
{
  return ctrl != NULL && ctrl->wave_active && queue_empty(ctrl->entry_queue);
}
