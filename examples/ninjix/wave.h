/**
 * @file wave.h
 * @brief Wave controller -- definitions, spawning logic, and game pacing.
 * @ingroup group_waves
 *
 * Each wave consists of one or more entries (enemy kind + count + burst
 * parameters). The wave controller manages per-wave timers, burst
 * scheduling, spawn-entry queuing, and the overall level-complete condition.
 */

#ifndef PROJ_WAVE_H
#define PROJ_WAVE_H

#include <stdbool.h>
#include <stdint.h>

#include "queue.h"
#include "enemy_pool.h"
#include "enemy_types.h"

typedef struct {
  enemy_kind_t kind;
  uint8_t count;
  uint8_t burst_size;
  uint16_t burst_interval;
  uint16_t spawn_interval;
  uint16_t burst_remaining;
  uint16_t spawned;
} wave_entry_t;

typedef struct wave {
  const wave_entry_t *entries;
  uint8_t entry_count;
  uint16_t pre_wave_delay;
} wave_t;

typedef struct {
  const wave_t *waves;
  uint16_t wave_count;
  uint16_t current_wave;
  queue_t *entry_queue;
  uint16_t timer;
  enemy_pool_t enemies;
  const enemy_type_t *enemy_types[ENEMY_KIND_COUNT];
  int16_t spawn_x;
  int16_t spawn_y;
  int16_t *path_x;
  int16_t *path_y;
  uint16_t path_len;
  uint16_t wave_delay;
  bool wave_active;
  bool level_complete;
} wave_controller_t;

/**
 * @brief Initialise the wave controller for a level.
 * @param ctrl         Controller to initialise.
 * @param waves        Array of wave definitions.
 * @param wave_count   Number of waves in the level.
 * @param enemy_types  Per-kind enemy type definitions.
 * @param spawn_x      World X coordinate of the enemy spawn point.
 * @param spawn_y      World Y coordinate of the enemy spawn point.
 * @param path_x       Precomputed X waypoints for path following.
 * @param path_y       Precomputed Y waypoints for path following.
 * @param path_len     Number of waypoints.
 */
void wave_ctrl_init(wave_controller_t *ctrl, const wave_t *waves, uint16_t wave_count,
                    const enemy_type_t enemy_types[ENEMY_KIND_COUNT],
                    int16_t spawn_x, int16_t spawn_y,
                    int16_t *path_x, int16_t *path_y, uint16_t path_len);

/**
 * @brief Release resources held by the wave controller (enemy pool, entry queue).
 * @param ctrl  Controller to tear down.
 */
void wave_ctrl_destroy(wave_controller_t *ctrl);

/**
 * @brief Advance the wave controller by one tick.
 *
 * Handles pre-wave countdown, burst scheduling, enemy spawning, and
 * detecting when all enemies are cleared to start the next wave.
 *
 * @param ctrl  Active wave controller.
 */
void wave_ctrl_step(wave_controller_t *ctrl);

/**
 * @brief Skip the pre-wave delay and start the next wave immediately.
 * @param ctrl  Active wave controller.
 */
void wave_ctrl_skip_delay(wave_controller_t *ctrl);

/**
 * @brief Deactivate dead enemies in the pool (free their slots).
 * @param ctrl  Active wave controller.
 */
void wave_ctrl_deactivate_inactive_enemies(wave_controller_t *ctrl);

/**
 * @brief Check whether the controller is waiting for all enemies to be cleared.
 * @param ctrl  Active wave controller.
 * @return true if the current wave is spawned but enemies remain, false otherwise.
 */
bool wave_ctrl_waiting_for_clear(const wave_controller_t *ctrl);

#endif
