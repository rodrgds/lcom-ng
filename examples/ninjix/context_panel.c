#include "context_panel.h"

#include <stdio.h>
#include <string.h>

#include "video_gr_ext.h"
#include "scene.h"
#include "button.h"
#include "colors.h"
#include "layout.h"
#include "panel.h"
#include "play_state.h"
#include "profiler.h"
#include "resources.h"
#include "sprite_utils.h"
#include "tips.h"
#include "tower_field.h"

#define CONTEXT_PREVIEW_Y_OFFSET 34
#define CONTEXT_PREVIEW_HEIGHT 64
#define CONTEXT_TARGET_MODE_Y (SIDEBAR_CONTEXT_Y + 188)

#define TARGET_MODE_STEP_COUNT 6
#define TARGET_MODE_STEP_SIZE 10
#define TARGET_MODE_STEP_GAP 4

static const char *tower_kind_name(tower_kind_t kind)
{
  switch (kind)
  {
    case TOWER_KIND_BASIC:  return "NORMAL";
    case TOWER_KIND_GUN:    return "SNIPER";
    case TOWER_KIND_SMOKE:  return "SMOKE";
    case TOWER_KIND_POISON: return "POISON";
    default:                return "";
  }
}

static const char *target_mode_name(target_mode_t mode)
{
  switch (mode)
  {
    case TARGET_FIRST:      return "FIRST";
    case TARGET_LAST:       return "LAST";
    case TARGET_CLOSEST:    return "CLOSEST";
    case TARGET_STRONGEST:  return "STRONGEST";
    case TARGET_WEAKEST:    return "WEAKEST";
    case TARGET_REWARD:     return "REWARD";
    default:                return "";
  }
}

static int scene_draw_target_mode_selector(scene_t *scene, uint16_t card_x,
                                             uint16_t card_width, uint16_t steps_y,
                                             uint8_t current_mode)
{
  const uint16_t steps_total = (uint16_t)(TARGET_MODE_STEP_COUNT * TARGET_MODE_STEP_SIZE
                                           + (TARGET_MODE_STEP_COUNT - 1) * TARGET_MODE_STEP_GAP);
  const uint16_t steps_x = (uint16_t)(card_x + (card_width - steps_total) / 2U);

  for (uint8_t i = 0; i < TARGET_MODE_STEP_COUNT; i++)
  {
    uint16_t sx = (uint16_t)(steps_x + i * (TARGET_MODE_STEP_SIZE + TARGET_MODE_STEP_GAP));
    uint32_t fill = (i == current_mode) ? COLOR_GRASS_LIGHT : COLOR_CARD_INSET;
    if (scene_fill_frame(sx, steps_y, TARGET_MODE_STEP_SIZE, TARGET_MODE_STEP_SIZE,
                         COLOR_TEXT_DARK, fill) != 0)
      return 1;
  }

  int16_t arrow_y = (int16_t)(steps_y + 1);
  int16_t left_x = (int16_t)steps_x - 8;
  if (scene_draw_text_colored(scene, "<", left_x, arrow_y,
                               BODY_SCALE, COLOR_TEXT_DARK) != 0)
    return 1;

  int16_t right_x = (int16_t)(steps_x + steps_total + 2);
  if (scene_draw_text_colored(scene, ">", right_x, arrow_y,
                               BODY_SCALE, COLOR_TEXT_DARK) != 0)
    return 1;

  if (scene_draw_centered_text_colored(scene, target_mode_name((target_mode_t)current_mode),
                                        card_x, (uint16_t)(steps_y + TARGET_MODE_STEP_SIZE + 6),
                                        card_width, BODY_SCALE, COLOR_TEXT_DIM) != 0)
    return 1;
  return 0;
}

static const char *upgrade_indicator(int32_t current, int32_t next)
{
  if (current == 0)
    return "";
  int32_t diff = next - current;
  if (diff < 0) diff = -diff;
  int32_t pct = diff * 100 / current;
  if (pct >= 50) return "+++";
  if (pct >= 25) return "++";
  if (pct >= 10) return "+";
  return "";
}

int scene_draw_tower_context_panel(scene_t *scene,
                                    uint16_t card_x, uint16_t context_y,
                                    uint16_t card_width, uint16_t context_height,
                                    uint16_t tower_index)
{
  const tower_t *tower = &scene->play.tower_field.towers[tower_index];
  const tower_type_t *type = tower->type;
  if (type == NULL)
    return 0;

  const uint16_t preview_width = 92;
  const uint16_t preview_height = 64;
  const uint16_t preview_x = (uint16_t)(card_x + (card_width - preview_width) / 2U);
  const uint16_t preview_y = (uint16_t)(context_y + 34);
  const uint16_t preview_center_x = (uint16_t)(preview_x + preview_width / 2);
  const uint16_t preview_center_y = (uint16_t)(preview_y + preview_height / 2);

  if (panel_draw(card_x, context_y, card_width, context_height) != 0)
    return 1;

  if (scene_draw_centered_text_colored(scene, tower_kind_name(type->kind), card_x,
                                       (uint16_t)(context_y + 14),
                                       card_width, BODY_SCALE, COLOR_TEXT_DARK) != 0)
    return 1;

  if (panel_draw(preview_x, preview_y, preview_width, preview_height) != 0)
    return 1;

  int16_t sprite_x = (int16_t)(preview_center_x - type->frame_width / 2);
  int16_t sprite_y = (int16_t)(preview_center_y - type->frame_height / 2);
  if (vg_draw_sprite_frame_scaled_to_buffer(type->sheet,
                                             type->frame_width, type->frame_height,
                                             0, 0,
                                              sprite_x, sprite_y, 1) != 0)
    return 1;

  uint16_t lvl_text_y = (uint16_t)(preview_y + preview_height + 12);
  if (tower->level < TOWER_MAX_LEVEL)
  {
    char lvl_progress[16];
    snprintf(lvl_progress, sizeof(lvl_progress), "LVL %u > %u", tower->level, tower->level + 1);
    if (scene_draw_centered_text_colored(scene, lvl_progress, card_x,
                                          lvl_text_y,
                                          card_width, BODY_SCALE, COLOR_TEXT_DARK) != 0)
      return 1;
  }

  const uint16_t upgrade_card_x = (uint16_t)(card_x + 12);
  const uint16_t upgrade_card_width = (uint16_t)(card_width - 24);
  const uint16_t upgrade_card_height = 36;
  const uint16_t upgrade_card_y = (uint16_t)(lvl_text_y + 22);
  if (panel_draw(upgrade_card_x, upgrade_card_y, upgrade_card_width, upgrade_card_height) != 0)
    return 1;
  if (tower->level < TOWER_MAX_LEVEL)
  {
    const tower_type_t *next_type = &scene->play.tower_field.tower_types[tower->level][type->kind];

    uint16_t stats_center = (uint16_t)(upgrade_card_x + upgrade_card_width / 2);
    int16_t stats_start_x = (int16_t)stats_center - 51;
    if (scene_draw_text_colored(scene, "DMG", stats_start_x, (int16_t)(upgrade_card_y + 8),
                                 BODY_SCALE, COLOR_TEXT_DARK) != 0)
      return 1;
    if (scene_draw_text_colored(scene, upgrade_indicator(type->damage, next_type->damage),
                                 (int16_t)(stats_start_x + 18), (int16_t)(upgrade_card_y + 8),
                                 BODY_SCALE, COLOR_GRASS_LIGHT) != 0)
      return 1;
    if (scene_draw_text_colored(scene, "RNG", (int16_t)(stats_start_x + 36), (int16_t)(upgrade_card_y + 8),
                                 BODY_SCALE, COLOR_TEXT_DARK) != 0)
      return 1;
    if (scene_draw_text_colored(scene, upgrade_indicator(type->range, next_type->range),
                                 (int16_t)(stats_start_x + 54), (int16_t)(upgrade_card_y + 8),
                                 BODY_SCALE, COLOR_GRASS_LIGHT) != 0)
      return 1;
    if (scene_draw_text_colored(scene, "SPD", (int16_t)(stats_start_x + 72), (int16_t)(upgrade_card_y + 8),
                                 BODY_SCALE, COLOR_TEXT_DARK) != 0)
      return 1;
    if (scene_draw_text_colored(scene, upgrade_indicator(type->cooldown_ticks, next_type->cooldown_ticks),
                                 (int16_t)(stats_start_x + 90), (int16_t)(upgrade_card_y + 8),
                                 BODY_SCALE, COLOR_GRASS_LIGHT) != 0)
      return 1;
    uint16_t upgrade_cost = next_type->cost;
    char cost_text[16];
    snprintf(cost_text, sizeof(cost_text), "COST %u", (unsigned)upgrade_cost);
    if (scene_draw_centered_text_colored(scene, cost_text, upgrade_card_x,
                                          (uint16_t)(upgrade_card_y + 20),
                                          upgrade_card_width, BODY_SCALE, COLOR_TEXT_DIM) != 0)
      return 1;
  }
  else
  {
    if (scene_draw_centered_text_colored(scene, "MAX LEVEL", upgrade_card_x,
                                          (uint16_t)(upgrade_card_y + 14),
                                          upgrade_card_width, BODY_SCALE, COLOR_TEXT_DARK) != 0)
      return 1;
  }

  uint16_t steps_y = (uint16_t)(upgrade_card_y + upgrade_card_height + 20);
  if (scene_draw_target_mode_selector(scene, card_x, card_width, steps_y,
                                       (uint8_t)tower->target_mode) != 0)
    return 1;

  const uint16_t btn_w = (uint16_t)(34U * UI_BUTTON_SCALE);
  const uint16_t btn_h = (uint16_t)(14U * UI_BUTTON_SCALE);
  const uint16_t btn_y = (uint16_t)(context_y + context_height - 36);
  const uint16_t btn_gap = 8;
  const uint16_t two_btns_w = (uint16_t)(btn_w * 2 + btn_gap);
  const uint16_t two_btns_x = (uint16_t)(card_x + (card_width - two_btns_w) / 2U);
  const uint16_t sell_btn_x = (uint16_t)(two_btns_x + btn_w + btn_gap);

  char upgrade_text[16];
  snprintf(upgrade_text, sizeof(upgrade_text), "UPGRADE");
  if (scene_draw_button(scene,
                         scene_get_sidebar_button_sprite(scene, false, two_btns_x,
                                                         btn_y, btn_w, btn_h),
                         two_btns_x, btn_y,
                         upgrade_text, COLOR_TEXT_DARK) != 0)
    return 1;

  char sell_text[16];
  uint16_t total_invested = 0;
  for (uint8_t lvl = 0; lvl < tower->level; lvl++)
    total_invested += scene->play.tower_field.tower_types[lvl][type->kind].cost;
  uint16_t refund = total_invested / 2;
  snprintf(sell_text, sizeof(sell_text), "SELL %u", (unsigned)refund);
  if (scene_draw_button(scene,
                         scene_get_sidebar_button_sprite(scene, true, sell_btn_x,
                                                         btn_y, btn_w, btn_h),
                         sell_btn_x, btn_y,
                         sell_text, COLOR_TEXT_SOFT) != 0)
    return 1;

  return 0;
}

static int scene_cache_empty_context(scene_t *scene, uint16_t x, uint16_t y,
                                      uint16_t width, uint16_t height)
{
  if (scene->empty_context_cached)
    return 0;

  if (scene->empty_context_sprite.pixels == NULL)
  {
    if (sprite_utils_create(&scene->empty_context_sprite, width, height) != 0)
      return 1;
  }

  if (scene->empty_context_sprite.width != width ||
      scene->empty_context_sprite.height != height)
    return 1;

  if (vg_capture_region_to_sprite(&scene->empty_context_sprite, x, y) != 0)
    return 1;

  scene->empty_context_cached = true;
  return 0;
}

int scene_draw_empty_context_panel(scene_t *scene,
                                    uint16_t card_x,
                                    uint16_t context_y,
                                    uint16_t card_width,
                                    uint16_t context_height)
{
  if (scene->empty_context_cached)
  {
    PROFILE_BEGIN("empty_context_cache_blit");
    int result = vg_opaque_sprite_to_buffer(&scene->empty_context_sprite, card_x, context_y);
    PROFILE_END();
    return result;
  }

  const uint16_t preview_width = 92;
  const uint16_t preview_height = 64;
  const uint16_t preview_x = (uint16_t)(card_x + (card_width - preview_width) / 2U);
  const uint16_t preview_y = (uint16_t)(context_y + 38);

  PROFILE_BEGIN("empty_context_build");
  if (panel_draw(card_x, context_y, card_width, context_height) != 0)
  {
    PROFILE_END();
    return 1;
  }
  if (scene_draw_centered_text_colored(scene, "NO TOWER SELECTED", card_x,
                                       (uint16_t)(context_y + 18),
                                       card_width, BODY_SCALE, COLOR_TEXT_DARK) != 0)
  {
    PROFILE_END();
    return 1;
  }
  if (panel_draw(preview_x, preview_y, preview_width, preview_height) != 0)
  {
    PROFILE_END();
    return 1;
  }
  if (scene_draw_centered_text_colored(scene, "?", preview_x,
                                       (uint16_t)(preview_y + 18),
                                       preview_width, HUD_SCALE * 2,
                                       COLOR_TEXT_DARK) != 0)
  {
    PROFILE_END();
    return 1;
  }
  if (scene_draw_centered_text_colored(scene, "SELECT A TOWER", card_x,
                                       (uint16_t)(preview_y + preview_height + 14),
                                       card_width, BODY_SCALE, COLOR_TEXT_DIM) != 0)
  {
    PROFILE_END();
    return 1;
  }
  if (scene_draw_centered_text_colored(scene, "OR BUY FROM SHOP.", card_x,
                                       (uint16_t)(preview_y + preview_height + 30),
                                       card_width, BODY_SCALE,                                  COLOR_TEXT_DIM) != 0)
  {
    PROFILE_END();
    return 1;
  }

  const uint16_t tips_x = (uint16_t)(card_x + 12);
  const uint16_t tips_y = (uint16_t)(context_y + 162);
  const uint16_t tips_width = (uint16_t)(card_width - 24);
  const uint16_t tips_height = 96;
  if (scene_draw_tips_panel(scene, tips_x, tips_y, tips_width, tips_height, false) != 0)
  {
    PROFILE_END();
    return 1;
  }
  PROFILE_END();

  PROFILE_BEGIN("empty_context_capture");
  (void)scene_cache_empty_context(scene, card_x, context_y, card_width, context_height);
  PROFILE_END();
  return 0;
}
