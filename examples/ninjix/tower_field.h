/**
 * @file tower_field.h
 * @brief Tower field -- manages placed towers and tower type definitions.
 * @ingroup group_towers
 *
 * Owns the tower_type_t lookup table (per-level, per-kind stats) and the
 * array of placed tower_t instances.  Provides add/remove/step operations
 * and initialises all tower types from the sprite bank.
 */

#ifndef PROJ_TOWER_FIELD_H
#define PROJ_TOWER_FIELD_H

#include <stdint.h>

#include "tower.h"

typedef struct {
  const vg_sprite_t *basic_sheets[TOWER_MAX_LEVEL];
  const vg_sprite_t *gun_sheets[TOWER_MAX_LEVEL];
  const vg_sprite_t *smoke_sheets[TOWER_MAX_LEVEL];
  const vg_sprite_t *poison_sheets[TOWER_MAX_LEVEL];
} tower_sprite_bank_t;

typedef struct {
  tower_type_t tower_types[TOWER_MAX_LEVEL][TOWER_KIND_COUNT];
  tower_t towers[TOWER_MAX_COUNT];
  uint16_t tower_count;
} tower_field_t;

/**
 * @brief Initialise the tower field -- populates tower types and clears placed towers.
 *
 * Validates spritesheets, fills the tower_types[][] table with scaled stats,
 * and initialises animation descriptors for each tower kind.
 *
 * @param field       Tower field to initialise.
 * @param sprites     Sprite bank with per-kind spritesheets (one per level).
 * @return 0 on success, 1 if a spritesheet is invalid.
 */
int tower_field_init(tower_field_t *field, const tower_sprite_bank_t *sprites);

/**
 * @brief Place a new tower of the given kind at the specified pixel position.
 * @param field  Tower field to add to.
 * @param kind   Tower kind to place.
 * @param x      Top-left X pixel coordinate.
 * @param y      Top-left Y pixel coordinate.
 * @return 0 on success, 1 if the field is full.
 */
int tower_field_add(tower_field_t *field, tower_kind_t kind, int16_t x, int16_t y);

/**
 * @brief Remove a tower by index, shifting subsequent towers down.
 * @param field  Tower field.
 * @param index  Index of the tower to remove (0-based).
 * @return 0 on success, 1 if index is out of bounds.
 */
int tower_field_remove(tower_field_t *field, uint16_t index);

/**
 * @brief Advance all placed towers by one tick (cooldown, animation).
 * @param field  Tower field whose towers to step.
 */
void tower_field_step(tower_field_t *field);

#endif
