/**
 * @file tower.h
 * @brief Tower entity -- type definitions, runtime state, and animation.
 * @ingroup group_towers
 *
 * A tower_type_t defines a tower kind's stats (range, damage, cooldown, cost,
 * effects, projectile kind) and spritesheet layout.  A tower_t is a placed
 * instance that tracks position, direction, targeting, cooldown, and animation.
 */

#ifndef PROJ_TOWER_H
#define PROJ_TOWER_H

#include <stdbool.h>
#include <stdint.h>

#include "video_gr_ext.h"
#include "enemy_handle.h"
#include "projectile_kind.h"

#define TOWER_FRAME_WIDTH 32
#define TOWER_FRAME_HEIGHT 32
#define TOWER_FRAME_COLUMNS 13
#define TOWER_GUN_FRAME_WIDTH 48
#define TOWER_GUN_FRAME_HEIGHT 48
#define TOWER_GUN_FRAME_COLUMNS 9
#define TOWER_POISON_FRAME_COLUMNS 12
#define TOWER_DIRECTION_ROWS 6
#define TOWER_MAX_COUNT 64
#define TOWER_MAX_LEVEL 5

#define TOWER_IDLE_FRAME_TICKS 30
#define TOWER_ATTACK_FRAME_TICKS 3

typedef enum {
  TOWER_KIND_BASIC = 0,
  TOWER_KIND_GUN,
  TOWER_KIND_SMOKE,
  TOWER_KIND_POISON,
  TOWER_KIND_COUNT
} tower_kind_t;

typedef enum {
  TOWER_DIR_DOWN = 0,
  TOWER_DIR_DOWN_RIGHT,
  TOWER_DIR_DOWN_LEFT,
  TOWER_DIR_UP,
  TOWER_DIR_UP_RIGHT,
  TOWER_DIR_UP_LEFT
} tower_dir_t;

typedef enum {
  TOWER_ANIM_IDLE = 0,
  TOWER_ANIM_ATTACK
} tower_anim_t;

typedef enum {
  TARGET_FIRST = 0,
  TARGET_LAST,
  TARGET_CLOSEST,
  TARGET_STRONGEST,
  TARGET_WEAKEST,
  TARGET_REWARD
} target_mode_t;

typedef struct {
  uint8_t start_col;
  uint8_t end_col;
  uint8_t loop;
  uint8_t release_col;
} tower_anim_desc_t;

typedef enum {
  TOWER_EFFECT_NONE = 0,
  TOWER_EFFECT_SLOW,
  TOWER_EFFECT_POISON,
} tower_effect_t;

typedef struct {
  tower_kind_t kind;
  const vg_sprite_t *sheet;
  uint16_t frame_width;
  uint16_t frame_height;
  uint8_t frame_columns;
  uint8_t direction_rows;
  uint16_t range;
  uint16_t damage;
  uint16_t cooldown_ticks;
  uint16_t cost;
  uint32_t highlight_color;
  projectile_kind_t projectile_kind;
  uint16_t projectile_release_ticks;
  uint8_t effect_type;
  uint16_t effect_duration;
  uint8_t effect_strength;
  tower_anim_desc_t idle_anim;
  tower_anim_desc_t attack_anim;
} tower_type_t;

typedef struct {
  const tower_type_t *type;
  int16_t x;
  int16_t y;
  tower_dir_t direction;
  tower_anim_t anim;
  target_mode_t target_mode;
  enemy_handle_t pending_target;
  uint16_t anim_tick;
  uint16_t cooldown_left;
  uint8_t frame_col;
  uint8_t level;
} tower_t;

/**
 * @brief Populate a tower_type_t with stats, spritesheet, and animation descriptors.
 *
 * Typically called once per tower kind during tower_field_init().  All
 * numeric parameters are stored directly; no allocation occurs.
 *
 * @param type                   Type struct to fill.
 * @param kind                   Tower kind enum (BASIC, GUN, SMOKE, POISON).
 * @param sheet                  Spritesheet with idle and attack frames.
 * @param frame_width            Width of a single frame in pixels.
 * @param frame_height           Height of a single frame in pixels.
 * @param frame_columns          Number of animation columns in the spritesheet.
 * @param direction_rows         Number of direction rows (typically 6).
 * @param range                  Targeting range in pixels.
 * @param damage                 Damage per projectile.
 * @param cooldown_ticks         Ticks between consecutive shots.
 * @param cost                   Coin cost to place this tower.
 * @param highlight_color        Range-circle highlight colour (RGB).
 * @param projectile_kind        Projectile type fired by this tower.
 * @param projectile_release_ticks  Tick in the attack animation to release the projectile.
 * @param effect_type            Status effect (TOWER_EFFECT_NONE / SLOW / POISON).
 * @param effect_duration        Effect duration in ticks.
 * @param effect_strength        Effect strength (0-255).
 * @param idle_anim              Idle animation descriptor.
 * @param attack_anim            Attack animation descriptor.
 */
void tower_type_init(tower_type_t *type, tower_kind_t kind, const vg_sprite_t *sheet,
                    uint16_t frame_width, uint16_t frame_height,
                    uint8_t frame_columns, uint8_t direction_rows,
                    uint16_t range, uint16_t damage, uint16_t cooldown_ticks,
                    uint16_t cost, uint32_t highlight_color, projectile_kind_t projectile_kind,
                    uint16_t projectile_release_ticks,
                    uint8_t effect_type, uint16_t effect_duration, uint8_t effect_strength,
                    const tower_anim_desc_t *idle_anim, const tower_anim_desc_t *attack_anim);

/**
 * @brief Initialise a placed tower instance.
 * @param tower  Tower struct to fill.
 * @param type   Tower type (stats/spritesheet).  Must outlive the tower.
 * @param x      Top-left X pixel coordinate on the map.
 * @param y      Top-left Y pixel coordinate on the map.
 */
void tower_init(tower_t *tower, const tower_type_t *type, int16_t x, int16_t y);

/**
 * @brief Rotate the tower to face a world-space point.
 * @param tower  Tower to rotate.
 * @param x      Target X pixel coordinate.
 * @param y      Target Y pixel coordinate.
 */
void tower_face_point(tower_t *tower, int16_t x, int16_t y);

/**
 * @brief Advance the tower's cooldown and animation by one tick.
 *
 * Decrements cooldown_left.  When the tower is in attack animation,
 * advances the frame column and returns to idle when the animation ends.
 *
 * @param tower  Tower to step.
 */
void tower_step(tower_t *tower);

#endif
