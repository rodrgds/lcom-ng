#include "tower.h"

#include <stddef.h>
#include <stdlib.h>

static const tower_anim_desc_t *tower_current_anim_desc(const tower_t *tower)
{
  if (tower == NULL || tower->type == NULL)
    return NULL;

  switch (tower->anim)
  {
    case TOWER_ANIM_ATTACK:
      return &tower->type->attack_anim;
    case TOWER_ANIM_IDLE:
    default:
      return &tower->type->idle_anim;
  }
}

static void tower_update_idle_animation(tower_t *tower)
{
  if (tower == NULL)
    return;

  const tower_anim_desc_t *anim = tower_current_anim_desc(tower);
  if (anim == NULL)
    return;

  uint8_t frame_count = (uint8_t)(anim->end_col - anim->start_col + 1);
  if (frame_count == 0)
  {
    tower->frame_col = anim->start_col;
    return;
  }

  uint16_t frame_index = (uint16_t)((tower->anim_tick / TOWER_IDLE_FRAME_TICKS) % frame_count);
  tower->frame_col = (uint8_t)(anim->start_col + frame_index);
}

void tower_type_init(tower_type_t *type, tower_kind_t kind, const vg_sprite_t *sheet,
                      uint16_t frame_width, uint16_t frame_height,
                      uint8_t frame_columns, uint8_t direction_rows,
                      uint16_t range, uint16_t damage, uint16_t cooldown_ticks,
                      uint16_t cost, uint32_t highlight_color, projectile_kind_t projectile_kind,
                      uint16_t projectile_release_ticks,
                      uint8_t effect_type, uint16_t effect_duration, uint8_t effect_strength,
                      const tower_anim_desc_t *idle_anim, const tower_anim_desc_t *attack_anim)
{
  if (type == NULL || idle_anim == NULL || attack_anim == NULL)
    return;

  type->kind = kind;
  type->sheet = sheet;
  type->frame_width = frame_width;
  type->frame_height = frame_height;
  type->frame_columns = frame_columns;
  type->direction_rows = direction_rows;
  type->range = range;
  type->damage = damage;
  type->cooldown_ticks = cooldown_ticks;
  type->cost = cost;
  type->highlight_color = highlight_color;
  type->projectile_kind = projectile_kind;
  type->projectile_release_ticks = projectile_release_ticks;
  type->effect_type = effect_type;
  type->effect_duration = effect_duration;
  type->effect_strength = effect_strength;
  type->idle_anim = *idle_anim;
  type->attack_anim = *attack_anim;
}

void tower_init(tower_t *tower, const tower_type_t *type, int16_t x, int16_t y)
{
  if (tower == NULL)
    return;

  tower->type = type;
  tower->x = x;
  tower->y = y;
  tower->direction = TOWER_DIR_DOWN;
  tower->anim = TOWER_ANIM_IDLE;
  tower->target_mode = TARGET_FIRST;
  tower->pending_target = enemy_handle_invalid();
  tower->anim_tick = 0;
  tower->cooldown_left = 0;
  tower->frame_col = (type != NULL) ? type->idle_anim.start_col : 0;
  tower->level = 1;
}

void tower_face_point(tower_t *tower, int16_t x, int16_t y)
{
  if (tower == NULL || tower->type == NULL)
    return;

  int16_t center_x = (int16_t)(tower->x + tower->type->frame_width / 2);
  int16_t center_y = (int16_t)(tower->y + tower->type->frame_height / 2);
  int16_t dx = (int16_t)(x - center_x);
  int16_t dy = (int16_t)(y - center_y);
  int16_t abs_dx = (int16_t)abs((int32_t)dx);
  int16_t abs_dy = (int16_t)abs((int32_t)dy);

  if (abs_dx < 20 && abs_dy > abs_dx * 2)
    // Enemy is nearly directly above or below the tower.
    tower->direction = (dy >= 0) ? TOWER_DIR_DOWN : TOWER_DIR_UP;
  else if (dy >= 0)
    // if the enemy is above us
    tower->direction = (dx >= 0) ? TOWER_DIR_DOWN_RIGHT : TOWER_DIR_DOWN_LEFT;
  else
    tower->direction = (dx >= 0) ? TOWER_DIR_UP_RIGHT : TOWER_DIR_UP_LEFT;
}

void tower_step(tower_t *tower)
{
  if (tower == NULL || tower->type == NULL)
    return;

  tower->anim_tick++;
  if (tower->cooldown_left > 0)
    tower->cooldown_left--;

  if (tower->anim == TOWER_ANIM_IDLE)
    tower_update_idle_animation(tower);
  else
  {
    const tower_anim_desc_t *anim = tower_current_anim_desc(tower);
    uint8_t frame_count = (uint8_t)(anim->end_col - anim->start_col + 1);
    uint16_t frame_index = (uint16_t)(tower->anim_tick / TOWER_ATTACK_FRAME_TICKS);

    if (frame_index >= frame_count)
    {
      // we've finished the attack animation, go back to idle
      tower->anim = TOWER_ANIM_IDLE;
      tower->anim_tick = 0;
      tower->pending_target = enemy_handle_invalid();
      tower_update_idle_animation(tower);
      return;
    }

    tower->frame_col = (uint8_t)(anim->start_col + frame_index);
  }
}

