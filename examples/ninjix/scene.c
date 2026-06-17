#include "scene.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"

#include "app_constants.h"
#include "colors.h"
#include "context_panel.h"
#include "enemy.h"
#include "enemy_card.h"
#include "enemy_path.h"
#include "enemy_types.h"
#include "overlay.h"
#include "panel.h"
#include "profiler.h"
#include "shop.h"
#include "sidebar.h"
#include "sprite_utils.h"
#include "text_utils.h"
#include "wave.h"

#define SCENE_PENDING_LIMIT 4096

static int scene_draw_autotiled_path_map(scene_t *scene)
{
  const uint16_t tile_size = MAP_TILE_SIZE;
  const uint16_t origin_x = MAP_ORIGIN_X;
  const uint16_t origin_y = MAP_ORIGIN_Y;

  for (uint16_t y = 0; y < scene->play.path_map.height; y++)
  {
    for (uint16_t x = 0; x < scene->play.path_map.width; x++)
    {
      if (!path_map_tile_is_path(path_map_get_tile(&scene->play.path_map, x, y)))
        continue;

      // Pick the road piece that matches the four neighbors around this cell.
      autotile_pick_t pick = path_map_pick_autotile(&scene->play.path_map, x, y);
      uint16_t src_x = (uint16_t)(pick.coords.grid_x * tile_size);
      uint16_t src_y = (uint16_t)(pick.coords.grid_y * tile_size);
      uint16_t dst_x = (uint16_t)(origin_x + x * tile_size);
      uint16_t dst_y = (uint16_t)(origin_y + y * tile_size);

      if (sprite_utils_draw_scaled(&scene->map_sprite, &scene->resources->grass_tileset_sprite,
                                    src_x, src_y, tile_size, tile_size,
                                    dst_x, dst_y, 1) != 0)
      {
        printf("scene_draw_autotiled_path_map(): failed at tile %u,%u using source %u,%u\n",
               (unsigned)x, (unsigned)y, (unsigned)src_x, (unsigned)src_y);
        return 1;
      }
    }
  }

  return 0;
}

static void scene_get_cursor_bounds(const scene_t *scene, int16_t *max_x, int16_t *max_y)
{
  int32_t bounded_max_x = (int32_t)get_h_res() - (int32_t)scene->resources->cursor_sprite.width;
  int32_t bounded_max_y = (int32_t)get_v_res() - (int32_t)scene->resources->cursor_sprite.height;

  *max_x = util_clamp_i16(bounded_max_x, 0, INT16_MAX);
  *max_y = util_clamp_i16(bounded_max_y, 0, INT16_MAX);
}

int scene_draw_text_colored(scene_t *scene, const char *text, int16_t x, int16_t y,
                            uint16_t scale, uint32_t color)
{
  return hud_font_draw_text_colored(&scene->resources->font, text, x, y, scale, color);
}

static int scene_draw_text_outline(scene_t *scene, const char *text,
                                   int16_t x, int16_t y, uint16_t scale,
                                   uint32_t outline_color, uint32_t fill_color)
{
  static const int8_t offsets[8][2] = {
    {-1, -1}, {0, -1}, {1, -1},
    {-1, 0},           {1, 0},
    {-1, 1},  {0, 1},  {1, 1}
  };

  for (size_t i = 0; i < sizeof(offsets) / sizeof(offsets[0]); i++)
  {
    int16_t offset_x = (int16_t)(x + offsets[i][0]);
    int16_t offset_y = (int16_t)(y + offsets[i][1]);
    if (scene_draw_text_colored(scene, text, offset_x, offset_y, scale, outline_color) != 0)
      return 1;
  }

  return scene_draw_text_colored(scene, text, x, y, scale, fill_color);
}

int scene_draw_centered_text_colored(scene_t *scene, const char *text,
                                     uint16_t left, uint16_t top, uint16_t width,
                                     uint16_t scale, uint32_t color)
{
  uint16_t text_width = text_utils_estimate_width(text, scale);
  int16_t text_x = (int16_t)left + (int16_t)(width / 2U) - (int16_t)(text_width / 2U);

  if (text_x < (int16_t)left + 4)
    text_x = (int16_t)left + 4;

  return scene_draw_text_colored(scene, text, text_x, (int16_t)top, scale, color);
}

/*
* Draw a good ol' rectangle with a border
*/
int scene_fill_frame(uint16_t x, uint16_t y, uint16_t width, uint16_t height,
                     uint32_t border_color, uint32_t fill_color)
{
  if (width < 4 || height < 4)
    return 1;

  if (vg_fill_rect_to_buffer(x, y, width, height, border_color) != 0)
    return 1;

  return vg_fill_rect_to_buffer((uint16_t)(x + 2), (uint16_t)(y + 2),
                                 (uint16_t)(width - 4), (uint16_t)(height - 4),
                                 fill_color);
}

static int scene_build_map_sprite(scene_t *scene)
{
  uint16_t map_height = (uint16_t)get_v_res();

  if (sprite_utils_create(&scene->map_sprite, MAP_WIDTH, map_height) != 0)
  {
    printf("scene_build_map_sprite(): failed to create %ux%u map sprite\n",
           (unsigned)MAP_WIDTH, (unsigned)map_height);
    return 1;
  }
  if (sprite_utils_fill_rect(&scene->map_sprite, 0, 0, MAP_WIDTH, map_height, COLOR_MAP_BORDER) != 0)
  {
    printf("scene_build_map_sprite(): failed to fill map border\n");
    return 1;
  }
  if (sprite_utils_fill_rect(&scene->map_sprite, 8, 8, (uint16_t)(MAP_WIDTH - 16), (uint16_t)(map_height - 16),
                             COLOR_GRASS_BACKDROP) != 0)
  {
    printf("scene_build_map_sprite(): failed to fill grass backdrop\n");
    return 1;
  }

  for (uint16_t y = 8; y + 31 < map_height; y = (uint16_t)(y + 32))
  {
    for (uint16_t x = 8; x + 31 < MAP_WIDTH; x = (uint16_t)(x + 32))
    {
      uint16_t tile_x = (uint16_t)(x / 32U);
      uint16_t tile_y = (uint16_t)(y / 32U);
      uint32_t hash = (uint32_t)(tile_x * 73856093U) ^ (uint32_t)(tile_y * 19349663U) ^ 0x9E3779B9U;
      
      /* Select a 32x32 grass tile from rows 6-8 (grid_y 5-7) of the tileset.
       * 17 base grass tiles total, excluding the transparent overlay at R8C6. 
       */

      uint16_t variant_index = (uint16_t)(hash % 17U);
      uint16_t variant_x = (uint16_t)((variant_index % 6U) * 32U);
      uint16_t variant_y = (uint16_t)((variant_index / 6U + 5U) * 32U);

      if (sprite_utils_draw_scaled(&scene->map_sprite, &scene->resources->grass_tileset_sprite,
                                    variant_x, variant_y, 32, 32, x, y, 1) != 0)
      {
        printf("scene_build_map_sprite(): failed to draw grass tile at %u,%u from %u,%u\n",
               (unsigned)x, (unsigned)y, (unsigned)variant_x, (unsigned)variant_y);
        return 1;
      }
    }
  }

  if (scene_draw_autotiled_path_map(scene) != 0)
  {
    printf("scene_build_map_sprite(): failed to draw path map\n");
    return 1;
  }

  return 0;
}

int scene_build_wave_panel_static(scene_t *scene, uint16_t card_x, uint16_t wave_y,
                                  uint16_t card_width, uint16_t wave_height)
{
  if (scene == NULL)
    return 1;

  if (scene->wave_panel_static.pixels == NULL)
  {
    if (sprite_utils_create(&scene->wave_panel_static, card_width, wave_height) != 0)
    {
      printf("scene_build_wave_panel_static(): failed to create %ux%u sprite\n",
             (unsigned)card_width, (unsigned)wave_height);
      return 1;
    }
  }
  else if (scene->wave_panel_static.width != card_width ||
           scene->wave_panel_static.height != wave_height)
  {
    vg_destroy_sprite(&scene->wave_panel_static);
    if (sprite_utils_create(&scene->wave_panel_static, card_width, wave_height) != 0)
    {
      printf("scene_build_wave_panel_static(): failed to recreate %ux%u sprite\n",
             (unsigned)card_width, (unsigned)wave_height);
      return 1;
    }
  }

  PROFILE_BEGIN("wave_panel_static_build");
  if (panel_draw(card_x, wave_y, card_width, wave_height) != 0)
  {
    PROFILE_END();
    return 1;
  }

  char level_text[16];
  snprintf(level_text, sizeof(level_text), "LEVEL %u", (unsigned)scene->play.level_id);
  if (scene_draw_text_colored(scene, level_text, (int16_t)(card_x + 16),
                               (int16_t)(wave_y + 16),
                               BODY_SCALE, COLOR_TEXT_DIM) != 0)
  {
    PROFILE_END();
    return 1;
  }

  const uint16_t progress_x = (uint16_t)(card_x + 16);
  const uint16_t progress_y = (uint16_t)(wave_y + 32);
  const uint16_t progress_w = 140;
  const uint16_t progress_h = 8;
  if (scene_fill_frame(progress_x, progress_y, progress_w, progress_h,
                        COLOR_TEXT_DARK, COLOR_CARD_INSET) != 0)
  {
    PROFILE_END();
    return 1;
  }

  if (scene->multiplayer_role == MULTIPLAYER_ROLE_DEFENDER)
  {
    const uint16_t active_bar_y = (uint16_t)(progress_y + 32);
    if (scene_fill_frame(progress_x, active_bar_y, progress_w, progress_h,
                          COLOR_TEXT_DARK, COLOR_CARD_INSET) != 0)
    {
      PROFILE_END();
      return 1;
    }

    if (scene_draw_text_colored(scene, "ENEMIES", (int16_t)(progress_x),
                                 (int16_t)(active_bar_y - 12),
                                 BODY_SCALE, COLOR_TEXT_DIM) != 0)
    {
      PROFILE_END();
      return 1;
    }
  }
  PROFILE_END();

  PROFILE_BEGIN("wave_panel_static_capture");
  if (vg_capture_region_to_sprite(&scene->wave_panel_static, card_x, wave_y) != 0)
  {
    PROFILE_END();
    return 1;
  }
  PROFILE_END();

  scene->wave_panel_static_cached = true;
  scene->wave_panel_static_level_id = scene->play.level_id;
  scene->wave_panel_static_defender = (scene->multiplayer_role == MULTIPLAYER_ROLE_DEFENDER);
  return 0;
}

static bool enemy_has_poison(const active_effect_t *effects, uint16_t slot_index)
{
  for (uint16_t i = 0; i < MAX_ACTIVE_EFFECTS; i++)
  {
    if (!effects[i].active)
      continue;
    if (effects[i].effect_type != TOWER_EFFECT_POISON)
      continue;
    if (effects[i].target.index != slot_index)
      continue;
    return true;
  }
  return false;
}

static int scene_draw_enemy_health_bar(const enemy_t *enemy, uint16_t slot_index,
                                        const active_effect_t *effects, const vg_sprite_t *poison_icon)
{
  if (enemy == NULL || enemy->type == NULL || !enemy->alive || effects == NULL || poison_icon == NULL)
    return 0;

  int16_t current = enemy->health;
  int16_t max_hp = enemy->type->health;

  if (current <= 0 || max_hp <= 0 || current >= max_hp)
    return 0;

  const uint16_t bar_width = 20;
  const uint16_t bar_height = 3;
  const uint16_t gap = 0;

  int16_t bar_x = (int16_t)(enemy->x - (int16_t)(bar_width / 2));
  int16_t bar_y = (int16_t)(enemy->y - (int16_t)(enemy->type->frame_height / 2) - (int16_t)(gap + bar_height));

  if (bar_x < 0 || bar_y < 0)
    return 0;

  if (vg_fill_rect_to_buffer((uint16_t)bar_x, (uint16_t)bar_y, bar_width, bar_height, 0x000000) != 0)
    return 1;

  uint16_t fill_width = (uint16_t)((uint32_t)current * bar_width / (uint32_t)max_hp);
  if (fill_width == 0)
    return 0;

  uint32_t color;
  uint32_t pct = (uint32_t)current * 100U / (uint32_t)max_hp;
  if (pct > 60U)
    color = 0x4CAF50;
  else if (pct > 30U)
    color = 0xFFC107;
  else
    color = 0xF44336;

  if (vg_fill_rect_to_buffer((uint16_t)bar_x, (uint16_t)bar_y, fill_width, bar_height, color) != 0)
    return 1;

  if (enemy_has_poison(effects, slot_index))
  {
    int16_t icon_x = (int16_t)(bar_x + bar_width + 2);
    int16_t icon_y = (int16_t)(bar_y - 1);
    vg_draw_sprite_region_scaled_to_buffer(poison_icon, 0, 0,
                                            poison_icon->width, poison_icon->height,
                                            icon_x, icon_y, 1);
  }

  return 0;
}

static int scene_draw_map_units(scene_t *scene)
{
  PROFILE_BEGIN("draw_towers");
  for (uint16_t index = 0; index < scene->play.tower_field.tower_count; index++)
  {
    const tower_t *tower = &scene->play.tower_field.towers[index];
    if (tower->type == NULL || tower->type->sheet == NULL || tower->type->sheet->pixels == NULL)
      continue;

    const tower_type_t *type = tower->type;
    if (type->frame_width == 0 || type->frame_height == 0 ||
        type->frame_columns == 0 || type->direction_rows == 0)
      continue;

    if ((uint32_t)type->frame_width * type->frame_columns > type->sheet->width ||
        (uint32_t)type->frame_height * type->direction_rows > type->sheet->height)
      continue;

    if (tower->frame_col >= type->frame_columns || tower->direction >= type->direction_rows)
      continue;

    if (vg_draw_sprite_frame_scaled_to_buffer(type->sheet,
                                               type->frame_width,
                                               type->frame_height,
                                               tower->frame_col,
                                               (uint16_t)tower->direction,
                                               tower->x,
                                               tower->y,
                                               1) != 0)
    {
      PROFILE_END();
      return 1;
    }
  }
  PROFILE_END();

  PROFILE_BEGIN("draw_enemies");
  for (uint16_t active_i = 0; active_i < scene->play.wave_ctrl.enemies.active_count; active_i++)
  {
    uint16_t i = scene->play.wave_ctrl.enemies.active_indices[active_i];
    const enemy_slot_t *slot = &scene->play.wave_ctrl.enemies.slots[i];
    if (!slot->active || !slot->enemy.alive)
      continue;

    const enemy_t *enemy = &slot->enemy;
    const enemy_type_t *etype = enemy->type;
    if (etype == NULL || etype->sheet == NULL || etype->sheet->pixels == NULL)
      continue;

    if (etype->abilities & ENEMY_ABILITY_INVISIBLE)
    {
      if (!enemy->revealed)
      {
        int16_t sx = (int16_t)(enemy->x - 2);
        int16_t sy = (int16_t)(enemy->y - 2);
        vg_fill_rect_to_buffer((uint16_t)sx, (uint16_t)(enemy->y), 5, 1, 0x848884);
        vg_fill_rect_to_buffer((uint16_t)(enemy->x), (uint16_t)sy, 1, 5, 0x848884);
        continue;
      }
    }

    vg_draw_sprite_frame_scaled_to_buffer(etype->sheet,
                                           etype->frame_width,
                                           etype->frame_height,
                                           enemy->frame_col,
                                           enemy->direction_row,
                                           (int16_t)(enemy->x - etype->frame_width / 2),
                                           (int16_t)(enemy->y - etype->frame_height / 2),
                                           1);
  }
  PROFILE_END();

  return 0;
}

static int scene_draw_enemy_health_bars(scene_t *scene)
{
  for (uint16_t active_i = 0; active_i < scene->play.wave_ctrl.enemies.active_count; active_i++)
  {
    uint16_t i = scene->play.wave_ctrl.enemies.active_indices[active_i];
    const enemy_slot_t *slot = &scene->play.wave_ctrl.enemies.slots[i];
    if (slot->active && slot->enemy.alive)
    {
      if (scene_draw_enemy_health_bar(&slot->enemy, i,
                                        scene->play.active_effects,
                                        &scene->resources->icon_poison_sprite) != 0)
        return 1;
    }
  }

  return 0;
}

static int scene_draw_projectiles(scene_t *scene)
{
  PROFILE_BEGIN("projectile_iterate");
  for (uint16_t active_i = 0; active_i < scene->play.projectiles.active_count; active_i++)
  {
    uint16_t i = scene->play.projectiles.active_indices[active_i];
    const projectile_t *projectile = &scene->play.projectiles.projectiles[i];
    if (!projectile->active || projectile->kind >= PROJECTILE_KIND_COUNT)
      continue;

    const vg_sprite_t *sprite = &scene->resources->projectile_sprites[projectile->kind];
    if (sprite->pixels == NULL)
      continue;

    PROFILE_BEGIN("projectile_sprite");
    if (vg_draw_sprite_region_scaled_to_buffer(sprite, 0, 0, sprite->width, sprite->height,
                                               (int16_t)(projectile->x - sprite->width / 2),
                                               (int16_t)(projectile->y - sprite->height / 2),
                                               1) != 0)
    {
      PROFILE_END();
      PROFILE_END();
      return 1;
    }
    PROFILE_END();
  }
  PROFILE_END();

  return 0;
}

static int scene_draw_resource_chip(scene_t *scene, const vg_sprite_t *icon,
                                     uint16_t x, uint16_t y, const char *value_text)
{
  if (vg_draw_sprite_region_scaled_to_buffer(icon, 0, 0, icon->width, icon->height,
                                             (int16_t)x, (int16_t)y, 2) != 0)
    return 1;

  return scene_draw_text_outline(scene, value_text, (int16_t)(x + 24), (int16_t)(y + 2), HUD_SCALE,
                                 COLOR_TEXT_DARK, COLOR_TEXT_SOFT);
}

static int scene_draw_top_hud(scene_t *scene)
{
  char hp_text[8];
  char coins_text[8];
  char elixir_text[8];
  snprintf(hp_text, sizeof(hp_text), "%u", (unsigned)scene->play.base_hp);
  snprintf(coins_text, sizeof(coins_text), "%u", (unsigned)scene->play.coins);
  snprintf(elixir_text, sizeof(elixir_text), "%u", (unsigned)scene->play.elixir);

  if (scene_draw_resource_chip(scene, &scene->resources->icon_health_sprite, 14, 12, hp_text) != 0)
    return 1;
  if (scene->multiplayer_role != MULTIPLAYER_ROLE_ATTACKER && scene_draw_resource_chip(scene, &scene->resources->icon_cash_sprite, 92, 12, coins_text) != 0)
    return 1;
  if (scene->multiplayer_role == MULTIPLAYER_ROLE_ATTACKER &&
      scene_draw_resource_chip(scene, &scene->resources->icon_elixir_sprite, 92, 12, elixir_text) != 0)
    return 1;
  return 0;
}

bool scene_point_in_rect(int16_t px, int16_t py,
  uint16_t rx, uint16_t ry,
  uint16_t rw, uint16_t rh)
{
  return px >= (int16_t)rx && py >= (int16_t)ry &&
         px < (int16_t)(rx + rw) && py < (int16_t)(ry + rh);
}

void scene_handle_placement_cancel(scene_t *scene)
{
  if (scene == NULL)
    return;

  play_state_cancel_placement(&scene->play);
}

static void scene_draw_range_circle(int16_t center_x, int16_t center_y, uint16_t r, uint32_t color)
{
  if (r == 0)
    return;

  int16_t f = 1 - (int16_t)r;
  int16_t ddF_x = 1;
  int16_t ddF_y = (int16_t)(-2 * (int16_t)r);
  int16_t px = 0;
  int16_t py = (int16_t)r;

  while (px <= py)
  {
    vg_fill_rect_to_buffer((uint16_t)(center_x + px), (uint16_t)(center_y + py), 1, 1, color);
    vg_fill_rect_to_buffer((uint16_t)(center_x + px), (uint16_t)(center_y - py), 1, 1, color);
    vg_fill_rect_to_buffer((uint16_t)(center_x - px), (uint16_t)(center_y + py), 1, 1, color);
    vg_fill_rect_to_buffer((uint16_t)(center_x - px), (uint16_t)(center_y - py), 1, 1, color);

    vg_fill_rect_to_buffer((uint16_t)(center_x + py), (uint16_t)(center_y + px), 1, 1, color);
    vg_fill_rect_to_buffer((uint16_t)(center_x + py), (uint16_t)(center_y - px), 1, 1, color);
    vg_fill_rect_to_buffer((uint16_t)(center_x - py), (uint16_t)(center_y + px), 1, 1, color);
    vg_fill_rect_to_buffer((uint16_t)(center_x - py), (uint16_t)(center_y - px), 1, 1, color);

    px++;
    if (f >= 0)
    {
      py--;
      ddF_y += 2;
      f += ddF_y;
    }
    ddF_x += 2;
    f += ddF_x;
  }
}

static int scene_draw_placement_preview(scene_t *scene)
{
  const play_state_t *play;

  if (scene == NULL)
    return 0;

  play = &scene->play;
  if (!play->placement_active || !play->placement_hover_valid)
    return 0;

  if (play->placement_hover_result != TILE_PLACEMENT_OK)
    return 0;

  const tower_type_t *type = &play->tower_field.tower_types[0][play->placement_kind];
  if (type->sheet == NULL || type->sheet->pixels == NULL)
    return 0;

  int16_t draw_x = (int16_t)(MAP_ORIGIN_X +
                               (uint16_t)play->placement_hover_col * MAP_TILE_SIZE +
                               (MAP_TILE_SIZE - type->frame_width) / 2);
  int16_t draw_y = (int16_t)(MAP_ORIGIN_Y +
                               (uint16_t)play->placement_hover_row * MAP_TILE_SIZE +
                               (MAP_TILE_SIZE - type->frame_height) / 2);

  int16_t center_x = (int16_t)(MAP_ORIGIN_X +
                                 (uint16_t)play->placement_hover_col * MAP_TILE_SIZE +
                                 MAP_TILE_SIZE / 2);
  int16_t center_y = (int16_t)(MAP_ORIGIN_Y +
                                 (uint16_t)play->placement_hover_row * MAP_TILE_SIZE +
                                 MAP_TILE_SIZE / 2);

  scene_draw_range_circle(center_x, center_y, type->range, type->highlight_color);

  return vg_draw_sprite_frame_scaled_to_buffer(type->sheet,
                                               type->frame_width,
                                               type->frame_height,
                                               type->idle_anim.start_col,
                                               0,
                                               draw_x,
                                               draw_y,
                                               1);
}

static int scene_draw_shrine(scene_t *scene)
{
  if (!scene->play.path_map.has_goal || scene->play.base_max_hp == 0)
    return 0;

  uint16_t shrine_x = (uint16_t)(MAP_ORIGIN_X + scene->play.path_map.goal_x * MAP_TILE_SIZE
                                  - (SHRINE_FRAME * 2 - MAP_TILE_SIZE) / 2);
  uint16_t shrine_y = (uint16_t)(MAP_ORIGIN_Y + scene->play.path_map.goal_y * MAP_TILE_SIZE
                                  - (SHRINE_FRAME * 2 - MAP_TILE_SIZE) / 2 - 8);

  uint16_t frame;
  uint32_t ratio = (uint32_t)scene->play.base_hp * 100U / (uint32_t)scene->play.base_max_hp;
  if (ratio > 60)
    frame = 0;
  else if (ratio > 30)
    frame = 1;
  else if (ratio > 0)
    frame = 2;
  else
    frame = 3;

  return vg_draw_sprite_region_scaled_to_buffer(&scene->resources->shrine_sprite,
                                                 (uint16_t)(frame * SHRINE_FRAME), 0,
                                                 SHRINE_FRAME, SHRINE_FRAME,
                                                 shrine_x, shrine_y, 2);
}

static int scene_draw_cursor(scene_t *scene)
{
  const vg_sprite_t *cursor;

  if (scene->play.placement_active)
  {
    if (!scene->play.placement_hover_valid ||
        scene->play.placement_hover_result != TILE_PLACEMENT_OK)
      cursor = &scene->resources->cursor_grab_sprite;
    else if (scene->play.coins <
             scene->play.tower_field.tower_types[0][scene->play.placement_kind].cost)
      cursor = &scene->resources->cursor_red_sprite;
    else
      cursor = &scene->resources->cursor_green_sprite;
  }
  else
  {
    cursor = &scene->resources->cursor_sprite;
  }

  return vg_draw_sprite_region_scaled_to_buffer(cursor, 0, 0,
                                                 cursor->width, cursor->height,
                                                 scene->cursor_x, scene->cursor_y, 1);
}

static int scene_render_gameplay_to_buffer(scene_t *scene)
{
  if (scene == NULL)
    return 1;

  PROFILE_BEGIN("draw_map");
  PROFILE_BEGIN("draw_map_cached_sprite");
  if (vg_opaque_sprite_to_buffer(&scene->map_sprite, 0, 0) != 0)
  {
    PROFILE_END();
    PROFILE_END();
    return 1;
  }
  PROFILE_END();
  PROFILE_END();

  if (scene_draw_shrine(scene) != 0)
    return 1;

  PROFILE_BEGIN("draw_units");
  if (scene_draw_map_units(scene) != 0)
  {
    PROFILE_END();
    return 1;
  }
  PROFILE_END();

  PROFILE_BEGIN("draw_projectiles");
  if (scene_draw_projectiles(scene) != 0)
  {
    PROFILE_END();
    return 1;
  }
  PROFILE_END();

  if (scene->play.selected_tower >= 0)
  {
    const tower_t *tower = &scene->play.tower_field.towers[scene->play.selected_tower];
    const tower_type_t *type = tower->type;
    if (type != NULL)
    {
      int16_t tower_center_x = (int16_t)(tower->x + type->frame_width / 2);
      int16_t tower_center_y = (int16_t)(tower->y + type->frame_height / 2);
      scene_draw_range_circle(tower_center_x, tower_center_y, type->range, type->highlight_color);
    }
  }

  if (scene_draw_placement_preview(scene) != 0)
    return 1;

  PROFILE_BEGIN("draw_hud");
  if (scene_draw_top_hud(scene) != 0)
  {
    PROFILE_END();
    return 1;
  }
  PROFILE_END();

  PROFILE_BEGIN("draw_sidebar");
  if (scene_draw_sidebar(scene) != 0)
  {
    PROFILE_END();
    return 1;
  }
  PROFILE_END();

  if (scene_draw_enemy_health_bars(scene) != 0)
    return 1;

  const wave_controller_t *ctrl = &scene->play.wave_ctrl;
  if (!ctrl->level_complete && !ctrl->wave_active && ctrl->timer > 0)
  {
    uint16_t seconds = (ctrl->timer + APP_TIMER_HZ - 1) / APP_TIMER_HZ;
    char countdown_text[24];
    snprintf(countdown_text, sizeof(countdown_text),
             "Wave %u in %u", (unsigned)(ctrl->current_wave + 1), (unsigned)seconds);

    uint16_t text_width = text_utils_estimate_width(countdown_text, HUD_SCALE);
    int16_t text_x = (int16_t)(MAP_ORIGIN_X + MAP_WIDTH / 2) - (int16_t)(text_width / 2);
    int16_t text_y = 50;

    if (scene_draw_text_outline(scene, countdown_text, text_x, text_y,
                                 HUD_SCALE, COLOR_TEXT_DARK, COLOR_TEXT_SOFT) != 0)
      return 1;
  }

  return 0;
}

int scene_init(scene_t *scene, const resources_t *resources, uint16_t level_id,
                uint16_t physics_hz, uint16_t render_hz)
{
  if (scene == NULL || resources == NULL || !resources->loaded)
    return 1;

  memset(scene, 0, sizeof(*scene));
  scene->resources = resources;
  scene->physics_hz = physics_hz;
  scene->render_hz = render_hz;

  if (play_state_init(&scene->play, resources, level_id) != 0)
  {
    printf("scene_init(): play_state_init() failed\n");
    scene_shutdown(scene);
    return 1;
  }

  if (scene_build_map_sprite(scene) != 0)
  {
    printf("scene_init(): scene_build_map_sprite() failed\n");
    scene_shutdown(scene);
    return 1;
  }

  if (scene_build_enemy_card_backgrounds(scene) != 0)
  {
    printf("scene_init(): scene_build_enemy_card_backgrounds() failed\n");
    scene_shutdown(scene);
    return 1;
  }

  int16_t max_x;
  int16_t max_y;
  scene_get_cursor_bounds(scene, &max_x, &max_y);

  scene->cursor_x = util_clamp_i16((int32_t)get_h_res() / 2, 0, max_x);
  scene->cursor_y = util_clamp_i16((int32_t)get_v_res() / 2, 0, max_y);
  scene->active_enemy_kind = -1;
  scene->display_fps = 0;
  scene->prev_left_button = false;

  return 0;
}

void scene_handle_mouse_click(scene_t *scene)
{
  tower_kind_t kind;

  if (scene == NULL)
    return;

  kind = scene_pick_shop_tower_kind(scene->cursor_x, scene->cursor_y);
  if (kind < TOWER_KIND_COUNT)
  {
    play_state_begin_placement(&scene->play, kind);
    return;
  }

  wave_controller_t *ctrl = &scene->play.wave_ctrl;
  if (!ctrl->wave_active && !ctrl->level_complete && ctrl->current_wave >= 0 &&
      scene_is_wave_status_hit(scene->cursor_x, scene->cursor_y))
  {
    wave_ctrl_skip_delay(ctrl);
    return;
  }

  if (scene_is_fast_forward_hit(scene->cursor_x, scene->cursor_y))
  {
    scene->play.speed_multiplier = (scene->play.speed_multiplier == 1) ? 2 : 1;
    return;
  }

  if (scene->play.selected_tower >= 0)
  {
    context_btn_t btn = scene_context_button_hit(scene->cursor_x, scene->cursor_y);
    if (btn == BTN_UPGRADE)
    {
      uint16_t index = (uint16_t)scene->play.selected_tower;
      tower_t *tower = &scene->play.tower_field.towers[index];
      const tower_type_t *type = tower->type;
      if (type != NULL && tower->level < TOWER_MAX_LEVEL)
      {
        const tower_type_t *next_type = &scene->play.tower_field.tower_types[tower->level][type->kind];
        uint16_t cost = next_type->cost;
        if (scene->play.coins >= cost)
        {
          scene->play.coins -= cost;
          tower->level++;
          tower->type = &scene->play.tower_field.tower_types[tower->level - 1][type->kind];
          scene->empty_context_cached = false;
        }
      }
      return;
    }
    if (btn == BTN_SELL)
    {
      uint16_t index = (uint16_t)scene->play.selected_tower;
      const tower_t *tower = &scene->play.tower_field.towers[index];
      if (tower->type != NULL)
      {
        uint16_t total_invested = 0;
        for (uint8_t lvl = 0; lvl < tower->level; lvl++)
          total_invested += scene->play.tower_field.tower_types[lvl][tower->type->kind].cost;
        scene->play.coins = (uint16_t)(scene->play.coins + total_invested / 2);
      }
      tower_field_remove(&scene->play.tower_field, index);
      scene->play.selected_tower = -1;
      scene->empty_context_cached = false;
      return;
    }
  }

  if (scene->play.selected_tower >= 0)
  {
    tower_t *tower = &scene->play.tower_field.towers[scene->play.selected_tower];
    int dir = scene_target_mode_arrow_hit(scene->cursor_x, scene->cursor_y);
    if (dir != 0)
    {
      int new_mode = (int)tower->target_mode + dir;
      if (new_mode < 0)
        new_mode = TARGET_REWARD;
      else if (new_mode > TARGET_REWARD)
        new_mode = TARGET_FIRST;
      tower->target_mode = (target_mode_t)new_mode;
      return;
    }
  }

  if (scene->cursor_x < MAP_WIDTH)
  {
    if (scene->play.placement_active)
    {
      (void)play_state_try_place_tower(&scene->play, scene->cursor_x, scene->cursor_y);
      return;
    }

    tower_field_t *field = &scene->play.tower_field;
    int16_t found = -1;
    for (int16_t i = (int16_t)field->tower_count - 1; i >= 0; i--)
    {
      const tower_t *tower = &field->towers[i];
      const tower_type_t *type = tower->type;
      if (type == NULL)
        continue;
      if (scene_point_in_rect(scene->cursor_x, scene->cursor_y,
                               (uint16_t)tower->x, (uint16_t)tower->y,
                               type->frame_width, type->frame_height))
      {
        found = i;
        break;
      }
    }

    scene->play.selected_tower = found;
    scene->empty_context_cached = false;
  }
}

void scene_set_multiplayer_role(scene_t *scene, multiplayer_role_t role)
{
  if (scene == NULL)
    return;

  scene->multiplayer_role = role;
  play_state_set_manual_spawning(&scene->play, role != MULTIPLAYER_ROLE_NONE);
}

void scene_handle_multiplayer_mouse_click(scene_t *scene,
                                           scene_multiplayer_action_t *action)
{
  enemy_kind_t enemy_kind;

  if (action != NULL)
    memset(action, 0, sizeof(*action));

  if (scene == NULL)
    return;

  if (scene->multiplayer_role == MULTIPLAYER_ROLE_ATTACKER)
  {
    enemy_kind = scene_pick_enemy_shop_kind(scene->cursor_x, scene->cursor_y);
    if (enemy_kind < ENEMY_KIND_COUNT)
    {
      scene->active_enemy_kind = (int16_t)enemy_kind;
      if (play_state_spawn_enemy(&scene->play, enemy_kind) == 0)
      {
        if (action != NULL)
        {
          action->type = MULTIPLAYER_EVENT_ENEMY_SPAWNED;
          action->data.enemy.kind = enemy_kind;
          action->data.enemy.wave = scene->play.wave_ctrl.current_wave;
        }
      }
    }
    return;
  }

  if (scene->multiplayer_role != MULTIPLAYER_ROLE_DEFENDER)
  {
    scene_handle_mouse_click(scene);
    return;
  }

  if (scene_is_wave_status_hit(scene->cursor_x, scene->cursor_y) ||
      scene_is_fast_forward_hit(scene->cursor_x, scene->cursor_y))
    return;

  bool was_placing = scene->play.placement_active;
  tower_kind_t placed_kind = scene->play.placement_kind;
  uint16_t previous_count = scene->play.tower_field.tower_count;
  int16_t selected_before = scene->play.selected_tower;
  uint8_t previous_level = 0;
  target_mode_t previous_mode = TARGET_FIRST;

  if (selected_before >= 0 && (uint16_t)selected_before < previous_count)
  {
    const tower_t *tower = &scene->play.tower_field.towers[selected_before];
    previous_level = tower->level;
    previous_mode = tower->target_mode;
  }

  scene_handle_mouse_click(scene);

  if (was_placing && scene->play.tower_field.tower_count > previous_count)
  {
    const tower_t *tower = &scene->play.tower_field.towers[scene->play.tower_field.tower_count - 1];
    if (action != NULL)
    {
      action->type = MULTIPLAYER_EVENT_TOWER_PLACED;
      action->data.tower.kind = placed_kind;
      action->data.tower.x = tower->x;
      action->data.tower.y = tower->y;
    }
    return;
  }

  if (selected_before < 0 || (uint16_t)selected_before >= previous_count || action == NULL)
    return;

  if (scene->play.tower_field.tower_count < previous_count)
  {
    action->type = MULTIPLAYER_EVENT_TOWER_SOLD;
    action->data.tower_sell.index = (uint16_t)selected_before;
    return;
  }

  if ((uint16_t)selected_before < scene->play.tower_field.tower_count)
  {
    const tower_t *tower = &scene->play.tower_field.towers[selected_before];
    if (tower->level != previous_level)
    {
      action->type = MULTIPLAYER_EVENT_TOWER_UPGRADED;
      action->data.tower_upgrade.index = (uint16_t)selected_before;
      action->data.tower_upgrade.level = tower->level;
      return;
    }

    if (tower->target_mode != previous_mode)
    {
      action->type = MULTIPLAYER_EVENT_TOWER_TARGET_CHANGED;
      action->data.tower_target.index = (uint16_t)selected_before;
      action->data.tower_target.mode = (uint8_t)tower->target_mode;
    }
  }
}

int scene_apply_remote_tower(scene_t *scene, tower_kind_t kind, int16_t x, int16_t y)
{
  if (scene == NULL)
    return 1;

  return play_state_place_tower_direct(&scene->play, kind, x, y);
}

int scene_apply_remote_tower_upgrade(scene_t *scene, uint16_t index, uint8_t level)
{
  if (scene == NULL || index >= scene->play.tower_field.tower_count ||
      level == 0 || level > TOWER_MAX_LEVEL)
    return 1;

  tower_t *tower = &scene->play.tower_field.towers[index];
  if (tower->type == NULL)
    return 1;

  tower_kind_t kind = tower->type->kind;
  tower->level = level;
  tower->type = &scene->play.tower_field.tower_types[level - 1][kind];
  scene->empty_context_cached = false;
  return 0;
}

int scene_apply_remote_tower_sell(scene_t *scene, uint16_t index)
{
  if (scene == NULL || index >= scene->play.tower_field.tower_count)
    return 1;

  if (tower_field_remove(&scene->play.tower_field, index) != 0)
    return 1;

  if (scene->play.selected_tower == (int16_t)index)
    scene->play.selected_tower = -1;
  else if (scene->play.selected_tower > (int16_t)index)
    scene->play.selected_tower--;

  scene->empty_context_cached = false;
  return 0;
}

int scene_apply_remote_tower_target(scene_t *scene, uint16_t index, uint8_t mode)
{
  if (scene == NULL || index >= scene->play.tower_field.tower_count || mode > TARGET_REWARD)
    return 1;

  scene->play.tower_field.towers[index].target_mode = (target_mode_t)mode;
  scene->empty_context_cached = false;
  return 0;
}

int scene_apply_remote_enemy(scene_t *scene, enemy_kind_t kind, uint16_t wave)
{
  if (scene == NULL)
    return 1;

  return play_state_spawn_enemy_in_wave(&scene->play, kind, wave);
}

void scene_shutdown(scene_t *scene)
{
  if (scene == NULL)
    return;

  vg_destroy_sprite(&scene->map_sprite);
  vg_destroy_sprite(&scene->empty_context_sprite);
  vg_destroy_sprite(&scene->enemy_card_bg_normal);
  vg_destroy_sprite(&scene->enemy_card_bg_hover);
  vg_destroy_sprite(&scene->enemy_card_bg_active);
  vg_destroy_sprite(&scene->wave_panel_static);
  scene->empty_context_cached = false;
  scene->wave_panel_static_cached = false;
  play_state_shutdown(&scene->play);
}

void scene_accumulate_mouse_motion(scene_t *scene, int16_t dx, int16_t dy,
                                    bool x_overflow, bool y_overflow)
{
  if (scene == NULL)
    return;

  if (x_overflow || y_overflow)
    return;

  scene->pending_dx = util_clamp_i32(scene->pending_dx + dx,
                                     -SCENE_PENDING_LIMIT, SCENE_PENDING_LIMIT);
  scene->pending_dy = util_clamp_i32(scene->pending_dy + dy,
                                     -SCENE_PENDING_LIMIT, SCENE_PENDING_LIMIT);
}

void scene_accumulate_mouse_packet(scene_t *scene, const struct packet *packet)
{
  if (packet == NULL)
    return;

  scene_accumulate_mouse_motion(scene, (int16_t)packet->delta_x, (int16_t)packet->delta_y,
                                packet->x_ov, packet->y_ov);
}

void scene_step_cursor(scene_t *scene)
{
  if (scene == NULL)
    return;

  int16_t max_x;
  int16_t max_y;
  scene_get_cursor_bounds(scene, &max_x, &max_y);

  int32_t next_x = (int32_t)scene->cursor_x + scene->pending_dx;
  int32_t next_y = (int32_t)scene->cursor_y - scene->pending_dy;

  scene->cursor_x = util_clamp_i16(next_x, 0, max_x);
  scene->cursor_y = util_clamp_i16(next_y, 0, max_y);
  scene->pending_dx = 0;
  scene->pending_dy = 0;
}

void scene_step_physics(scene_t *scene)
{
  if (scene == NULL)
    return;

  scene_step_cursor(scene);
  play_state_update_placement_hover(&scene->play, scene->cursor_x, scene->cursor_y);
  for (uint8_t i = 0; i < scene->play.speed_multiplier; i++)
    play_state_step(&scene->play);
}

typedef int (*overlay_draw_fn_t)(scene_t *scene, void *userdata);

static int scene_render_with_overlay(scene_t *scene, overlay_draw_fn_t draw_overlay,
                                      void *userdata, const char *label)
{
  PROFILE_BEGIN(label);
  if (scene_render_gameplay_to_buffer(scene) != 0)
  {
    PROFILE_END();
    return 1;
  }
  PROFILE_END();

  if (draw_overlay != NULL)
  {
    if (draw_overlay(scene, userdata) != 0)
      return 1;
  }

  PROFILE_BEGIN("draw_cursor");
  if (scene_draw_cursor(scene) != 0)
  {
    PROFILE_END();
    return 1;
  }
  PROFILE_END();

  PROFILE_BEGIN("present");
  int result = vg_present_buffer();
  PROFILE_END();
  return result;
}

typedef struct {
  const pause_menu_t *pause;
} pause_overlay_data_t;

static int draw_pause_overlay_wrapper(scene_t *scene, void *userdata)
{
  const pause_overlay_data_t *data = userdata;
  return scene_draw_pause_overlay(scene, data->pause);
}

typedef struct {
  const pause_menu_t *pause;
  const char *status_text;
} pause_multi_overlay_data_t;

static int draw_pause_multi_overlay_wrapper(scene_t *scene, void *userdata)
{
  const pause_multi_overlay_data_t *data = userdata;
  return scene_draw_pause_overlay_with_status(scene, data->pause, data->status_text);
}

typedef struct {
  const char *title;
  const overlay_menu_t *menu;
  const char *const *button_labels;
  const char *status_text;
} result_overlay_data_t;

static int draw_result_overlay_wrapper(scene_t *scene, void *userdata)
{
  const result_overlay_data_t *data = userdata;
  return scene_draw_result_overlay(scene, data->title, data->menu,
                                    data->button_labels, data->status_text);
}

int scene_render(scene_t *scene)
{
  return scene_render_with_overlay(scene, NULL, NULL, "scene");
}

int scene_render_paused(scene_t *scene, const pause_menu_t *pause)
{
  pause_overlay_data_t data = { .pause = pause };
  return scene_render_with_overlay(scene, draw_pause_overlay_wrapper, &data, "scene_paused");
}

int scene_render_paused_multiplayer(scene_t *scene, const pause_menu_t *pause,
                                     const char *status_text)
{
  pause_multi_overlay_data_t data = { .pause = pause, .status_text = status_text };
  return scene_render_with_overlay(scene, draw_pause_multi_overlay_wrapper, &data, "scene_paused_multi");
}

int scene_render_result(scene_t *scene, const char *title,
                         const overlay_menu_t *menu,
                         const char *const *button_labels,
                         const char *status_text)
{
  result_overlay_data_t data = { .title = title, .menu = menu,
                                  .button_labels = button_labels, .status_text = status_text };
  return scene_render_with_overlay(scene, draw_result_overlay_wrapper, &data, "scene_result");
}
