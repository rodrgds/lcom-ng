/**
 * @file enemy.c
 * @brief Enemy entity implementation -- initialisation, movement, and animation.
 * @ingroup group_enemies
 */

#include "enemy.h"

#include <stddef.h>
#include <stdlib.h>

void enemy_init(enemy_t *enemy, const enemy_type_t *type, int16_t start_x, int16_t start_y)
{
  if (enemy == NULL || type == NULL)
    return;

  enemy->type = type;
  enemy->x = start_x;
  enemy->y = start_y;
  enemy->wp_index = 1;
  enemy->health = type->health;
  enemy->anim_tick = 0;
  enemy->frame_col = type->idle_anim.start_col;
  enemy->direction_row = 0;
  enemy->alive = true;
  enemy->reached_goal = false;
  enemy->revealed = false;
  enemy->sub_pos = 0;
}

void enemy_step(enemy_t *enemy, const int16_t *path_x, const int16_t *path_y, uint16_t path_len, uint16_t override_speed)
{
  if (enemy == NULL || enemy->type == NULL || path_x == NULL || path_y == NULL)
    return;

  if (!enemy->alive || enemy->reached_goal)
    return;

  if (enemy->health <= 0)
  {
    enemy->alive = false;
    return;
  }

  int16_t tx = path_x[enemy->wp_index];
  int16_t ty = path_y[enemy->wp_index];

  int16_t dx = (int16_t)(tx - enemy->x);
  int16_t dy = (int16_t)(ty - enemy->y);

  int16_t abs_dx = (int16_t)abs((int32_t)dx);
  int16_t abs_dy = (int16_t)abs((int32_t)dy);

  if (abs_dx > abs_dy)
    enemy->direction_row = (dx > 0) ? 1 : 3;
  else if (abs_dy > 0)
    enemy->direction_row = (dy < 0) ? 0 : 2;

  uint16_t speed = (override_speed != UINT16_MAX) ? override_speed : enemy->type->speed;

  uint32_t acc = (uint32_t)enemy->sub_pos + speed;
  uint16_t steps = (uint16_t)(acc / 100U);
  enemy->sub_pos = (uint16_t)(acc % 100U);

  if (steps > 0)
  {
    if (abs_dx <= (int16_t)steps && abs_dy <= (int16_t)steps)
    {
      enemy->x = tx;
      enemy->y = ty;
      enemy->wp_index++;

      if (enemy->wp_index >= path_len)
      {
        enemy->reached_goal = true;
        enemy->alive = false;
        return;
      }
    }
    else
    {
      if (dx > 0)
        enemy->x = (int16_t)(enemy->x + steps);
      else if (dx < 0)
        enemy->x = (int16_t)(enemy->x - steps);

      if (dy > 0)
        enemy->y = (int16_t)(enemy->y + steps);
      else if (dy < 0)
        enemy->y = (int16_t)(enemy->y - steps);
    }
  }

  if (enemy->type->abilities & ENEMY_ABILITY_TELEPORTING &&
      enemy->anim_tick > 0 && enemy->anim_tick % 180 == 0)
  {
    uint16_t skip = 3;
    uint16_t land_wp = enemy->wp_index + skip;
    if (land_wp + 1 < path_len)
    {
      enemy->x = path_x[land_wp];
      enemy->y = path_y[land_wp];
      enemy->wp_index = land_wp + 1;
      enemy->sub_pos = 0;

      int16_t dx = (int16_t)(path_x[enemy->wp_index] - enemy->x);
      int16_t dy = (int16_t)(path_y[enemy->wp_index] - enemy->y);
      int16_t abs_dx = (int16_t)abs((int32_t)dx);
      int16_t abs_dy = (int16_t)abs((int32_t)dy);
      if (abs_dx > abs_dy)
        enemy->direction_row = (dx > 0) ? 1 : 3;
      else if (abs_dy > 0)
        enemy->direction_row = (dy < 0) ? 0 : 2;
    }
  }

  enemy->anim_tick++;

  const tower_anim_desc_t *anim = &enemy->type->idle_anim;
  uint8_t frame_count = (uint8_t)(anim->end_col - anim->start_col + 1);
  if (frame_count > 0)
  {
    uint16_t frame_index = (uint16_t)((enemy->anim_tick / enemy->type->anim_speed) % frame_count);
    enemy->frame_col = (uint8_t)(anim->start_col + frame_index);
  }
}

