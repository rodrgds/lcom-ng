/**
 * @file resources.h
 * @brief Resource handles -- all sprites, enemy types, and HUD font.
 * @ingroup group_resources
 *
 * resources_t owns every vg_sprite_t used by the game.  A single call to
 * resources_load() populates the struct; resources_destroy() releases it.
 */

#ifndef PROJ_RESOURCES_H
#define PROJ_RESOURCES_H

#include <stdbool.h>

#include "enemy_types.h"
#include "tower.h"
#include "hud_font.h"
#include "projectile_kind.h"

typedef enum {
  SHOP_SPRITE_NORMAL = 0,
  SHOP_SPRITE_HOVER,
  SHOP_SPRITE_PRESSED,
  SHOP_SPRITE_STATE_COUNT
} shop_sprite_state_t;

typedef struct resources {
  hud_font_t font;
  vg_sprite_t cursor_sprite;
  vg_sprite_t cursor_grab_sprite;
  vg_sprite_t cursor_green_sprite;
  vg_sprite_t cursor_red_sprite;
  vg_sprite_t shrine_sprite;
  vg_sprite_t grass_tileset_sprite;
  vg_sprite_t panel_sprite;
  vg_sprite_t menu_logo_sprite;
  vg_sprite_t tower_shop_sprites[TOWER_KIND_COUNT][SHOP_SPRITE_STATE_COUNT];
  vg_sprite_t button_green_sprite;
  vg_sprite_t button_green_hover_sprite;
  vg_sprite_t button_silver_sprite;
  vg_sprite_t button_silver_hover_sprite;
  vg_sprite_t icon_cash_sprite;
  vg_sprite_t icon_elixir_sprite;
  vg_sprite_t icon_health_sprite;
  vg_sprite_t icon_pause_sprite;
  vg_sprite_t icon_skull_full_sprite;
  vg_sprite_t icon_poison_sprite;
  vg_sprite_t tower_basic_sprites[TOWER_MAX_LEVEL];
  vg_sprite_t tower_gun_sprites[TOWER_MAX_LEVEL];
  vg_sprite_t tower_smoke_sprites[TOWER_MAX_LEVEL];
  vg_sprite_t tower_poison_sprites[TOWER_MAX_LEVEL];
  vg_sprite_t enemy_sprites[ENEMY_KIND_COUNT];
  vg_sprite_t projectile_sprites[PROJECTILE_KIND_COUNT];
  enemy_type_t enemy_types[ENEMY_KIND_COUNT];
  bool loaded;
} resources_t;

/**
 * @brief Load all game assets (sprites, enemy types, HUD font).
 *
 * Reads PNG/XPM files from proj/assets/ and proj/xpm/ and fills every
 * field in the resources struct.  Must be called once before any rendering.
 *
 * @param resources  Resources struct to fill.
 * @return 0 on success, non-zero on failure.
 */
int resources_load(resources_t *resources);

/**
 * @brief Release all resources and mark the struct as unloaded.
 * @param resources  Resources struct to tear down.
 */
void resources_destroy(resources_t *resources);

#endif
