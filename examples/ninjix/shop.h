/**
 * @file shop.h
 * @brief Tower and enemy shop card drawing, sprite lookup, and kind-picking.
 * @ingroup group_ui
 */

#ifndef PROJ_SHOP_H
#define PROJ_SHOP_H

#include <stdint.h>

#include "enemy_types.h"
#include "tower.h"

struct scene_t;

int scene_draw_tower_shop_card(struct scene_t *scene, tower_kind_t kind,
  const vg_sprite_t *sprite, uint16_t x, uint16_t y,
  const char *name_text);

int scene_draw_enemy_shop_card(struct scene_t *scene, enemy_kind_t kind,
  uint16_t x, uint16_t y, const char *name_text);

const vg_sprite_t *scene_get_tower_shop_sprite(const struct scene_t *scene,
  tower_kind_t kind);

tower_kind_t scene_pick_shop_tower_kind(int16_t cursor_x, int16_t cursor_y);

enemy_kind_t scene_pick_enemy_shop_kind(int16_t cursor_x, int16_t cursor_y);

#endif
