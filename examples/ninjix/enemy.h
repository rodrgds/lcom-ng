/**
 * @file enemy.h
 * @brief Enemy runtime state, movement, health, and status effects.
 * @ingroup group_enemies
 *
 * Each enemy instance follows a precomputed path of waypoints using
 * sub-pixel positioning. Enemies have a type (kind), animated spritesheet
 * with direction rows, and can be affected by slow and poison effects.
 */

#ifndef PROJ_ENEMY_H
#define PROJ_ENEMY_H

#include <stdbool.h>
#include <stdint.h>

#include "enemy_types.h"

typedef struct {
  const enemy_type_t *type;
  int16_t x;
  int16_t y;
  uint16_t wp_index;
  int16_t health;
  uint16_t anim_tick;
  uint8_t frame_col;
  uint8_t direction_row;
  bool alive;
  bool reached_goal;
  bool revealed;
  uint16_t sub_pos;
} enemy_t;

/**
 * @brief Initialise an enemy instance at a starting position.
 * @param enemy    Enemy struct to fill.
 * @param type     Enemy type definition (HP, speed, reward, abilities).
 * @param start_x  World X coordinate of the spawn point.
 * @param start_y  World Y coordinate of the spawn point.
 */
void enemy_init(enemy_t *enemy, const enemy_type_t *type, int16_t start_x, int16_t start_y);

/**
 * @brief Advance the enemy by one tick -- movement, animation, status effects.
 *
 * Moves the enemy toward the next waypoint using sub-pixel positioning.
 * If override_speed is non-zero, it replaces the type's base speed (used
 * by teleporting enemies).  Sets reached_goal when the final waypoint
 * is reached and alive=false when HP drops to zero.
 *
 * @param enemy          Enemy to step.
 * @param path_x         Precomputed X waypoints.
 * @param path_y         Precomputed Y waypoints.
 * @param path_len       Number of waypoints.
 * @param override_speed Speed override (UINT16_MAX = use type speed).
 */
void enemy_step(enemy_t *enemy, const int16_t *path_x, const int16_t *path_y, uint16_t path_len, uint16_t override_speed);

#endif
