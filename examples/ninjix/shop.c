#include "shop.h"

#include <stdio.h>
#include <string.h>

#include "scene.h"
#include "colors.h"
#include "layout.h"
#include "panel.h"
#include "play_state.h"
#include "resources.h"
#include "text_utils.h"

#define SHOP_CARD_WIDTH 70
#define SHOP_CARD_HEIGHT 76
#define SHOP_CARD_COL1_X (MAP_WIDTH + 24)
#define SHOP_CARD_COL2_X (MAP_WIDTH + 100)
#define SHOP_CARD_ROW1_Y 46
#define SHOP_CARD_ROW2_Y 128

#define ENEMY_CARD_WIDTH 72
#define ENEMY_CARD_HEIGHT 79

typedef struct {
  enemy_kind_t kind;
  uint8_t col;
  uint8_t row;
  const char *name;
} enemy_shop_card_t;

static const enemy_shop_card_t attacker_enemy_shop_cards[ENEMY_KIND_COUNT] = {
  { ENEMY_KIND_NORMAL,      0, 0, "NORMAL" },
  { ENEMY_KIND_FAST,        1, 0, "FAST" },
  { ENEMY_KIND_HEALING,     0, 1, "HEAL" },
  { ENEMY_KIND_ARMORED,     1, 1, "ARMOR" },
  { ENEMY_KIND_EXPLODING,   0, 2, "BOOM" },
  { ENEMY_KIND_CASH,        1, 2, "CASH" },
  { ENEMY_KIND_TELEPORTING, 0, 3, "BLINK" },
  { ENEMY_KIND_INVISIBLE,   1, 3, "HIDE" },
};

static int scene_draw_centered_icon_value(scene_t *scene, const vg_sprite_t *icon,
                                           uint16_t x, uint16_t y, uint16_t width,
                                           const char *value_text, uint32_t color)
{
  const uint16_t icon_width = icon->width;
  const uint16_t value_width = text_utils_estimate_width(value_text, BODY_SCALE);
  const uint16_t total_width = (uint16_t)(icon_width + value_width + 2);
  const uint16_t offset = (total_width < width) ? (uint16_t)((width - total_width) / 2U) : 0;
  const int16_t start_x = (int16_t)(x + offset);

  if (vg_draw_sprite_region_scaled_to_buffer(icon, 0, 0,
                                             icon_width, icon->height,
                                             start_x, (int16_t)(y + 2), 1) != 0)
    return 1;

  return scene_draw_text_colored(scene, value_text,
                                 (int16_t)(start_x + icon_width + 2),
                                 (int16_t)(y + 2), BODY_SCALE, color);
}

static int scene_draw_shop_card(scene_t *scene, const vg_sprite_t *sprite,
                                 const vg_sprite_t *price_icon,
                                 uint16_t x, uint16_t y,
                                 const char *name_text, const char *price_text)
{
  const uint16_t card_width = 70;
  const uint16_t card_height = 76;
  const uint16_t icon_scale = SHOP_ICON_SCALE;

  const uint16_t icon_width = (uint16_t)(sprite->width * icon_scale);
  const uint16_t icon_x = (uint16_t)(x + (card_width - icon_width) / 2U);
  const uint16_t icon_y = (uint16_t)(y + 4);

  const uint16_t name_y = (uint16_t)(y + 40);
  const uint16_t price_y = (uint16_t)(y + 54);

  if (panel_draw(x, y, card_width, card_height) != 0)
    return 1;

  if (vg_draw_sprite_region_scaled_to_buffer(sprite, 0, 0, sprite->width, sprite->height,
                                             (int16_t)icon_x, (int16_t)icon_y, icon_scale) != 0)
    return 1;

  if (scene_draw_centered_text_colored(scene, name_text, x, name_y,
                                       card_width, BODY_SCALE, COLOR_TEXT_DARK) != 0)
    return 1;

  return scene_draw_centered_icon_value(scene, price_icon, x, price_y,
                                        card_width, price_text, COLOR_TEXT_DARK);
}

int scene_draw_tower_shop_card(scene_t *scene, tower_kind_t kind,
                                const vg_sprite_t *sprite, uint16_t x, uint16_t y,
                                const char *name_text)
{
  char price_text[8];
  const tower_type_t *type = &scene->play.tower_field.tower_types[0][kind];
  snprintf(price_text, sizeof(price_text), "%u", (unsigned)type->cost);

  return scene_draw_shop_card(scene, sprite, &scene->resources->icon_cash_sprite,
                               x, y, name_text, price_text);
}

int scene_draw_enemy_shop_card(scene_t *scene, enemy_kind_t kind,
                                uint16_t x, uint16_t y,
                                const char *name_text)
{
  const uint16_t card_width  = ENEMY_CARD_WIDTH;
  const uint16_t card_height = ENEMY_CARD_HEIGHT;
  const enemy_type_t *type = &scene->resources->enemy_types[kind];
  const vg_sprite_t *sprite = type->sheet;
  const uint16_t remaining = play_state_multiplayer_enemy_remaining(&scene->play, kind);
  const uint16_t cost = play_state_enemy_elixir_cost(kind);
  const uint32_t text_color = (remaining > 0 && scene->play.elixir >= cost) ?
                               COLOR_TEXT_DARK : COLOR_TEXT_DIM;
  char cost_text[8];
  char remaining_text[12];
  const bool hovered = scene_point_in_rect(scene->cursor_x, scene->cursor_y, x, y, card_width, card_height);
  const bool active = (scene->active_enemy_kind == (int16_t)kind);

  snprintf(cost_text, sizeof(cost_text), "%u", (unsigned)cost);
  snprintf(remaining_text, sizeof(remaining_text), "%u LEFT", (unsigned)remaining);

  const vg_sprite_t *bg = active ? &scene->enemy_card_bg_active :
                         hovered ? &scene->enemy_card_bg_hover :
                                   &scene->enemy_card_bg_normal;
  if (vg_opaque_sprite_to_buffer(bg, (int16_t)x, (int16_t)y) != 0)
    return 1;

  if (vg_draw_sprite_frame_scaled_to_buffer(sprite, type->frame_width, type->frame_height,
                                             3, 2,
                                             (int16_t)(x + (card_width - type->frame_width) / 2U),
                                             (int16_t)(y + 4), 1) != 0)
    return 1;

  if (scene_draw_centered_text_colored(scene, name_text, x, (uint16_t)(y + 36),
                                       card_width, BODY_SCALE, text_color) != 0)
    return 1;

  if (scene_draw_centered_icon_value(scene, &scene->resources->icon_elixir_sprite,
                                     x, (uint16_t)(y + 48), card_width,
                                     cost_text, text_color) != 0)
    return 1;

  return scene_draw_centered_text_colored(scene, remaining_text, x, (uint16_t)(y + 64),
                                           card_width, BODY_SCALE, COLOR_TEXT_DIM);
}

const vg_sprite_t *scene_get_tower_shop_sprite(const scene_t *scene,
  tower_kind_t kind)
{
  if (scene == NULL || kind >= TOWER_KIND_COUNT)
    return NULL;

  tower_kind_t hovered_kind = scene_pick_shop_tower_kind(scene->cursor_x, scene->cursor_y);
  bool pressed = scene->play.placement_active && scene->play.placement_kind == kind;
  bool hovered = !pressed && hovered_kind == kind;

  shop_sprite_state_t state = pressed ? SHOP_SPRITE_PRESSED :
                              hovered  ? SHOP_SPRITE_HOVER :
                                         SHOP_SPRITE_NORMAL;
  return &scene->resources->tower_shop_sprites[kind][state];
}

tower_kind_t scene_pick_shop_tower_kind(int16_t cursor_x, int16_t cursor_y)
{
  if (scene_point_in_rect(cursor_x, cursor_y, SHOP_CARD_COL1_X, SHOP_CARD_ROW1_Y,
                          SHOP_CARD_WIDTH, SHOP_CARD_HEIGHT))
    return TOWER_KIND_BASIC;

  if (scene_point_in_rect(cursor_x, cursor_y, SHOP_CARD_COL2_X, SHOP_CARD_ROW1_Y,
                          SHOP_CARD_WIDTH, SHOP_CARD_HEIGHT))
    return TOWER_KIND_GUN;

  if (scene_point_in_rect(cursor_x, cursor_y, SHOP_CARD_COL1_X, SHOP_CARD_ROW2_Y,
                          SHOP_CARD_WIDTH, SHOP_CARD_HEIGHT))
    return TOWER_KIND_SMOKE;

  if (scene_point_in_rect(cursor_x, cursor_y, SHOP_CARD_COL2_X, SHOP_CARD_ROW2_Y,
                          SHOP_CARD_WIDTH, SHOP_CARD_HEIGHT))
    return TOWER_KIND_POISON;

  return TOWER_KIND_COUNT;
}

enemy_kind_t scene_pick_enemy_shop_kind(int16_t cursor_x, int16_t cursor_y)
{
  for (uint8_t i = 0; i < ENEMY_KIND_COUNT; i++)
  {
    const enemy_shop_card_t *card = &attacker_enemy_shop_cards[i];
    uint16_t x = (uint16_t)(((card->col == 0) ? SHOP_CARD_COL1_X : SHOP_CARD_COL2_X));
    uint16_t y = ATTACKER_ENEMY_ROW_Y(card->row);
    if (scene_point_in_rect(cursor_x, cursor_y, x, y, SHOP_CARD_WIDTH, SHOP_CARD_HEIGHT))
      return card->kind;
  }

  return ENEMY_KIND_COUNT;
}
