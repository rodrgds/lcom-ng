/**
 * @file enemy_types.h
 * @brief Enemy type definitions -- kinds, abilities, stats, and initialisation.
 * @ingroup group_enemies
 *
 * Eight enemy kinds are defined: normal, fast, healing, armored, exploding,
 * cash, teleporting, and invisible. Each type has health, speed, reward,
 * base damage, and a set of ability flags for special behaviour.
 */

#ifndef PROJ_ENEMY_TYPES_H
#define PROJ_ENEMY_TYPES_H

#include <stdint.h>

#include "video_gr_ext.h"
#include "tower.h"

typedef enum {
  ENEMY_KIND_NORMAL = 0,
  ENEMY_KIND_FAST,
  ENEMY_KIND_HEALING,
  ENEMY_KIND_ARMORED,
  ENEMY_KIND_EXPLODING,
  ENEMY_KIND_CASH,
  ENEMY_KIND_TELEPORTING,
  ENEMY_KIND_INVISIBLE,
  ENEMY_KIND_COUNT
} enemy_kind_t;

typedef enum {
  ENEMY_ABILITY_NONE        = 0,
  ENEMY_ABILITY_HEALING     = 1 << 0,
  ENEMY_ABILITY_EXPLODING   = 1 << 1,
  ENEMY_ABILITY_ARMORED     = 1 << 2,
  ENEMY_ABILITY_TELEPORTING = 1 << 3,
  ENEMY_ABILITY_INVISIBLE   = 1 << 4,
} enemy_ability_t;

typedef struct {
  enemy_kind_t kind;
  const vg_sprite_t *sheet;
  uint16_t frame_width;
  uint16_t frame_height;
  uint8_t frame_columns;
  uint8_t direction_rows;
  tower_anim_desc_t idle_anim;
  int16_t health;
  uint16_t speed;
  uint16_t anim_speed;
  uint16_t reward;
  uint16_t damage_to_base;
  uint8_t abilities;
} enemy_type_t;

/**
 * @brief Populate the enemy type table with stats and spritesheets.
 *
 * Fills all ENEMY_KIND_COUNT entries with predefined stats (HP, speed,
 * reward, abilities) and assigns spritesheets for each kind.
 *
 * @param types    Array of ENEMY_KIND_COUNT enemy_type_t structs to fill.
 * @param sprites  Array of ENEMY_KIND_COUNT spritesheets (one per kind).
 */
void enemy_types_init(enemy_type_t types[ENEMY_KIND_COUNT],
                      const vg_sprite_t *sprites[ENEMY_KIND_COUNT]);

#endif
