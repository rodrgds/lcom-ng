/**
 * @file enemy_types.c
 * @brief Enemy type definitions -- stat tables and initialisation for all eight enemy kinds.
 * @ingroup group_enemies
 */

#include "enemy_types.h"

static const tower_anim_desc_t idle_anim_full = {
  .start_col = 0,
  .end_col = 5,
  .loop = 1,
  .release_col = 0,
};

void enemy_types_init(enemy_type_t types[ENEMY_KIND_COUNT],
                      const vg_sprite_t *sprites[ENEMY_KIND_COUNT])
{
  struct {
    enemy_kind_t kind;
    int16_t health;
    uint16_t speed;
    uint16_t reward;
    uint16_t damage_to_base;
    uint8_t abilities;
  } const stats[] = {
    {ENEMY_KIND_NORMAL,      90,  70, 12, 10, ENEMY_ABILITY_NONE},
    {ENEMY_KIND_FAST,        60, 115, 14,  6, ENEMY_ABILITY_NONE},
    {ENEMY_KIND_HEALING,    130,  55, 18, 12, ENEMY_ABILITY_HEALING},
    {ENEMY_KIND_ARMORED,    170,  48, 26, 20, ENEMY_ABILITY_ARMORED},
    {ENEMY_KIND_EXPLODING,  110,  80, 20, 30, ENEMY_ABILITY_EXPLODING},
    {ENEMY_KIND_CASH,        70,  85, 45, 10, ENEMY_ABILITY_NONE},
    {ENEMY_KIND_TELEPORTING,110,  75, 24,  8, ENEMY_ABILITY_TELEPORTING},
    {ENEMY_KIND_INVISIBLE,   75,  85, 24, 10, ENEMY_ABILITY_INVISIBLE},
  };

  for (uint8_t i = 0; i < ENEMY_KIND_COUNT; i++)
  {
    types[i].kind = stats[i].kind;
    types[i].sheet = (sprites != NULL) ? sprites[i] : NULL;
    types[i].frame_width = 32;
    types[i].frame_height = 32;
    types[i].frame_columns = 6;
    types[i].direction_rows = 4;
    types[i].idle_anim = idle_anim_full;
    types[i].health = stats[i].health;
    types[i].speed = stats[i].speed;
    types[i].anim_speed = 12;
    types[i].reward = stats[i].reward;
    types[i].damage_to_base = stats[i].damage_to_base;
    types[i].abilities = stats[i].abilities;
  }
}
