#include "tips.h"

#include "video_gr_ext.h"
#include "colors.h"
#include "scene.h"
#include "panel.h"
#include "text_utils.h"

typedef enum {
  TIP_ICON_TARGET,
  TIP_ICON_CASH,
  TIP_ICON_SKULL,
  TIP_ICON_ELIXIR,
} tip_icon_t;

static int scene_draw_tip_target_icon(uint16_t x, uint16_t y)
{
  if (vg_fill_rect_to_buffer((uint16_t)(x + 7), y, 4, 2, COLOR_TEXT_DARK) != 0 ||
      vg_fill_rect_to_buffer((uint16_t)(x + 7), (uint16_t)(y + 16), 4, 2, COLOR_TEXT_DARK) != 0 ||
      vg_fill_rect_to_buffer(x, (uint16_t)(y + 7), 2, 4, COLOR_TEXT_DARK) != 0 ||
      vg_fill_rect_to_buffer((uint16_t)(x + 16), (uint16_t)(y + 7), 2, 4, COLOR_TEXT_DARK) != 0)
    return 1;

  if (vg_fill_rect_to_buffer((uint16_t)(x + 4), (uint16_t)(y + 4), 2, 2, COLOR_TEXT_DARK) != 0 ||
      vg_fill_rect_to_buffer((uint16_t)(x + 12), (uint16_t)(y + 4), 2, 2, COLOR_TEXT_DARK) != 0 ||
      vg_fill_rect_to_buffer((uint16_t)(x + 4), (uint16_t)(y + 12), 2, 2, COLOR_TEXT_DARK) != 0 ||
      vg_fill_rect_to_buffer((uint16_t)(x + 12), (uint16_t)(y + 12), 2, 2, COLOR_TEXT_DARK) != 0)
    return 1;

  if (vg_fill_rect_to_buffer((uint16_t)(x + 8), (uint16_t)(y + 5), 2, 8, COLOR_TEXT_DARK) != 0 ||
      vg_fill_rect_to_buffer((uint16_t)(x + 5), (uint16_t)(y + 8), 8, 2, COLOR_TEXT_DARK) != 0)
    return 1;

  if (vg_fill_rect_to_buffer((uint16_t)(x + 8), (uint16_t)(y + 8), 2, 2, COLOR_PANEL_FILL) != 0)
    return 1;

  return 0;
}

static int scene_draw_scaled_sprite_centered(const vg_sprite_t *sprite,
                                              uint16_t x, uint16_t y,
                                              uint16_t slot_width, uint16_t slot_height,
                                              uint16_t scale)
{
  if (sprite == NULL || sprite->pixels == NULL || scale == 0)
    return 1;

  const uint16_t draw_width = (uint16_t)(sprite->width * scale);
  const uint16_t draw_height = (uint16_t)(sprite->height * scale);

  int16_t draw_x = (int16_t)x;
  int16_t draw_y = (int16_t)y;

  if (slot_width > draw_width)
    draw_x = (int16_t)(x + (slot_width - draw_width) / 2U);

  if (slot_height > draw_height)
    draw_y = (int16_t)(y + (slot_height - draw_height) / 2U);

  return vg_draw_sprite_region_scaled_to_buffer(sprite, 0, 0,
                                         sprite->width, sprite->height,
                                         draw_x, draw_y, scale);
}

static int scene_draw_tip_row(scene_t *scene, tip_icon_t icon,
                               uint16_t x, uint16_t y, const char *text)
{
  const uint16_t icon_slot_width = 24;
  const uint16_t icon_slot_height = 18;

  const uint16_t icon_x = x;
  const uint16_t icon_y = y;
  const uint16_t text_x = (uint16_t)(x + icon_slot_width + 5);
  const uint16_t text_y = (uint16_t)(y + 3);

  switch (icon)
  {
    case TIP_ICON_TARGET:
      if (scene_draw_tip_target_icon((uint16_t)(icon_x + 3), icon_y) != 0)
        return 1;
      break;

    case TIP_ICON_CASH:
      if (scene_draw_scaled_sprite_centered(&scene->resources->icon_cash_sprite,
                                         icon_x, icon_y,
                                         icon_slot_width, icon_slot_height,
                                         2) != 0)
        return 1;
      break;

    case TIP_ICON_SKULL:
      if (scene_draw_scaled_sprite_centered(&scene->resources->icon_skull_full_sprite,
                                         icon_x, icon_y,
                                         icon_slot_width, icon_slot_height,
                                         2) != 0)
        return 1;
      break;
    case TIP_ICON_ELIXIR:
      if (scene_draw_scaled_sprite_centered(&scene->resources->icon_elixir_sprite,
                                         icon_x, icon_y,
                                         icon_slot_width, icon_slot_height,
                                         2) != 0)
        return 1;
      break;
  }

 return scene_draw_text_colored(scene, text,
                                (int16_t)text_x, (int16_t)text_y,
                                BODY_SCALE, COLOR_TEXT_DARK);
}

int scene_draw_tips_panel(scene_t *scene,
                          uint16_t x, uint16_t y,
                          uint16_t width, uint16_t height, bool isAttackingMultiplayer)
{
  const uint16_t title_y = (uint16_t)(y + 12);

  const uint16_t row_x = (uint16_t)(x + 8);

  const uint16_t row1_y = (uint16_t)(y + 32);
  const uint16_t row2_y = (uint16_t)(y + 53);
  const uint16_t row3_y = (uint16_t)(y + 74);

  if (panel_draw(x, y, width, height) != 0)
    return 1;

  if (scene_draw_centered_text_colored(scene, isAttackingMultiplayer ? "ATTACKING TIPS" : "TOWER TIPS", x, title_y,
                                       width, BODY_SCALE, COLOR_TEXT_DARK) != 0)
    return 1;

  if (scene_draw_tip_row(scene, TIP_ICON_TARGET, row_x, row1_y, isAttackingMultiplayer ? "SPREAD OVER WAVES" : "PLACE NEAR TURNS") != 0)
    return 1;
  if (scene_draw_tip_row(scene, isAttackingMultiplayer ? TIP_ICON_ELIXIR : TIP_ICON_CASH, row_x, row2_y, isAttackingMultiplayer ? "SPEND ALL ELIXIR" : "SAVE FOR UPGRADES") != 0)
    return 1;
  if (scene_draw_tip_row(scene, TIP_ICON_SKULL, row_x, row3_y, isAttackingMultiplayer ? "MIX ENEMY TYPES" : "STOP LEAKS EARLY") != 0)
    return 1;

  return 0;
}
