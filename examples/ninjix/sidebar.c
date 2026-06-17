#include "sidebar.h"

#include <stdio.h>

#include "video_gr_ext.h"
#include "scene.h"
#include "button.h"
#include "colors.h"
#include "context_panel.h"
#include "layout.h"
#include "panel.h"
#include "play_state.h"
#include "profiler.h"
#include "shop.h"
#include "text_utils.h"
#include "tips.h"

static int sidebar_draw_progress_bar(scene_t *scene,
                                      uint16_t card_x, uint16_t card_width,
                                      uint16_t bar_x, uint16_t bar_y,
                                      uint16_t bar_w, uint16_t bar_h,
                                      uint16_t fill_w, const char *label,
                                      const char *value)
{
  if (scene_draw_text_colored(scene, label, (int16_t)(bar_x),
                              (int16_t)(bar_y - 12),
                              BODY_SCALE, COLOR_TEXT_DIM) != 0)
    return 1;
  if (value != NULL)
  {
    uint16_t val_w = text_utils_estimate_width(value, BODY_SCALE);
    if (scene_draw_text_colored(scene, value,
                                (int16_t)(card_x + card_width - 15 - val_w),
                                (int16_t)(bar_y - 12),
                                BODY_SCALE, COLOR_TEXT_DARK) != 0)
      return 1;
  }
  if (scene_fill_frame(bar_x, bar_y, bar_w, bar_h,
                        COLOR_TEXT_DARK, COLOR_CARD_INSET) != 0)
    return 1;
  if (fill_w > 0)
  {
    if (vg_fill_rect_to_buffer((uint16_t)(bar_x + 2), (uint16_t)(bar_y + 2),
                                fill_w, (uint16_t)(bar_h - 4),
                                COLOR_GRASS_LIGHT) != 0)
      return 1;
  }
  return 0;
}

static bool scene_is_sidebar_button_hit(int16_t px, int16_t py, uint16_t col_offset)
{
  const uint16_t btn_x = (uint16_t)(SIDEBAR_CARD_X + col_offset);
  const uint16_t btn_y = (uint16_t)(SIDEBAR_WAVE_Y + 54);
  const uint16_t btn_w = (uint16_t)(34U * UI_BUTTON_SCALE);
  const uint16_t btn_h = (uint16_t)(14U * UI_BUTTON_SCALE);

  return scene_point_in_rect(px, py, btn_x, btn_y, btn_w, btn_h);
}

bool scene_is_wave_status_hit(int16_t px, int16_t py)
{
  return scene_is_sidebar_button_hit(px, py, 13);
}

bool scene_is_fast_forward_hit(int16_t px, int16_t py)
{
  return scene_is_sidebar_button_hit(px, py, 90);
}

int scene_target_mode_arrow_hit(int16_t px, int16_t py)
{
  const uint16_t steps_total = (uint16_t)(6 * 10 + (6 - 1) * 4);
  const uint16_t steps_x = (uint16_t)(SIDEBAR_CARD_X + (SIDEBAR_CARD_WIDTH - steps_total) / 2U);
  uint16_t arrow_y = (uint16_t)(SIDEBAR_CONTEXT_Y + 188 + 1);

  if (scene_point_in_rect(px, py, (uint16_t)(steps_x - 8), arrow_y, 6, 7))
    return -1;
  if (scene_point_in_rect(px, py, (uint16_t)(steps_x + steps_total + 2), arrow_y, 6, 7))
    return 1;
  return 0;
}

context_btn_t scene_context_button_hit(int16_t px, int16_t py)
{
  const uint16_t btn_w = (uint16_t)(34U * UI_BUTTON_SCALE);
  const uint16_t btn_h = (uint16_t)(14U * UI_BUTTON_SCALE);
  const uint16_t btn_gap = 8;
  const uint16_t two_btns_w = (uint16_t)(btn_w * 2 + btn_gap);
  const uint16_t two_btns_x = (uint16_t)(SIDEBAR_CARD_X + (SIDEBAR_CARD_WIDTH - two_btns_w) / 2U);
  const uint16_t btn_y = (uint16_t)(SIDEBAR_CONTEXT_Y + SIDEBAR_CONTEXT_HEIGHT - 36);

  if (scene_point_in_rect(px, py, two_btns_x, btn_y, btn_w, btn_h))
    return BTN_UPGRADE;
  if (scene_point_in_rect(px, py, (uint16_t)(two_btns_x + btn_w + btn_gap), btn_y, btn_w, btn_h))
    return BTN_SELL;
  return BTN_NONE;
}

int scene_draw_sidebar(scene_t *scene)
{
  if (scene->multiplayer_role == MULTIPLAYER_ROLE_ATTACKER)
  {
    const uint16_t sidebar_x = MAP_WIDTH;
    const uint16_t card_x = (uint16_t)(MAP_WIDTH + 12);
    const uint16_t card_width = 172;
    const uint16_t shop_y = 8;
    const uint16_t shop_height = ATTACKER_SHOP_HEIGHT + 108;
    const uint16_t status_y = (uint16_t)(shop_y + shop_height + 8);
    const uint16_t status_height = (uint16_t)(get_v_res() - status_y - 8);
    const uint16_t total_waves = scene->play.wave_ctrl.wave_count;
    uint16_t display_wave = (uint16_t)(scene->play.wave_ctrl.current_wave + 1);

    if (vg_fill_rect_to_buffer(sidebar_x, 0, SIDEBAR_WIDTH, (uint16_t)get_v_res(), COLOR_SIDEBAR_BG) != 0)
      return 1;
    if (vg_fill_rect_to_buffer(sidebar_x, 0, 4, (uint16_t)get_v_res(), COLOR_TEXT_DARK) != 0)
      return 1;

    if (panel_draw(card_x, shop_y, card_width, shop_height) != 0)
      return 1;
    if (panel_draw(card_x, status_y, card_width, status_height) != 0)
      return 1;

    if (scene_draw_centered_text_colored(scene, "ATTACK", card_x, 23, card_width,
                                         HUD_SCALE, COLOR_TEXT_DARK) != 0)
      return 1;

    for (uint8_t i = 0; i < ENEMY_KIND_COUNT; i++)
    {
      enemy_kind_t kind = (enemy_kind_t)i;
      const char *name;
      switch (kind)
      {
        case ENEMY_KIND_NORMAL:      name = "NORMAL"; break;
        case ENEMY_KIND_FAST:        name = "FAST"; break;
        case ENEMY_KIND_HEALING:     name = "HEAL"; break;
        case ENEMY_KIND_ARMORED:     name = "ARMOR"; break;
        case ENEMY_KIND_EXPLODING:   name = "BOOM"; break;
        case ENEMY_KIND_CASH:        name = "CASH"; break;
        case ENEMY_KIND_TELEPORTING: name = "BLINK"; break;
        case ENEMY_KIND_INVISIBLE:   name = "HIDE"; break;
        default:                     name = "???"; break;
      }
      uint8_t col = i % 2;
      uint8_t row = i / 2;
      uint16_t x = (uint16_t)(card_x + ((col == 0) ? 12 : 88));
      uint16_t y = ATTACKER_ENEMY_ROW_Y(row);
      if (scene_draw_enemy_shop_card(scene, kind, x, y, name) != 0)
        return 1;
    }

    const uint16_t tips_x      = (uint16_t)(card_x + 12);
    const uint16_t tips_y      = (uint16_t)(shop_y + ATTACKER_SHOP_HEIGHT + 2);
    const uint16_t tips_width  = (uint16_t)(card_width - 25);
    const uint16_t tips_height = 96;

    if (scene_draw_tips_panel(scene, tips_x, tips_y, tips_width, tips_height, true) != 0)
      return 1;

    const uint16_t wave_y = SIDEBAR_WAVE_Y;
    const uint16_t progress_x = (uint16_t)(card_x + 16);
    const uint16_t progress_w = 140;
    const uint16_t progress_h = 8;
    const uint16_t progress_inner_w = (uint16_t)(progress_w - 4);

    const uint16_t active_bar_y = (uint16_t)(wave_y + 16);
    const uint32_t active_count = (uint32_t)scene->play.wave_ctrl.enemies.active_count;
    const uint32_t active_max   = (uint32_t)play_state_multiplayer_wave_remaining_total(&scene->play) + active_count;
    const uint16_t active_fill_w = (active_max == 0) ? 0 :
        (uint16_t)((active_count * progress_inner_w) / active_max);

    char active_val[16];
    snprintf(active_val, sizeof(active_val), "%u", (unsigned)active_count);
    if (sidebar_draw_progress_bar(scene, card_x, card_width,
                                   progress_x, active_bar_y,
                                   progress_w, progress_h,
                                   active_fill_w, "ACTIVE", active_val) != 0)
      return 1;

    const uint16_t remaining_bar_y = (uint16_t)(active_bar_y + progress_h + 20);
    const uint32_t remaining_count = (uint32_t)play_state_multiplayer_wave_remaining_total(&scene->play);
    const uint32_t remaining_max   = active_max;
    const uint16_t remaining_fill_w = (remaining_max == 0) ? 0 :
        (uint16_t)((remaining_count * progress_inner_w) / remaining_max);

    char remaining_val[16];
    snprintf(remaining_val, sizeof(remaining_val), "%u", (unsigned)remaining_count);
    if (sidebar_draw_progress_bar(scene, card_x, card_width,
                                   progress_x, remaining_bar_y,
                                   progress_w, progress_h,
                                   remaining_fill_w, "LEFT", remaining_val) != 0)
      return 1;

    const uint16_t progress_y = (uint16_t)(remaining_bar_y + progress_h + 20);

    char level_text[16];
    snprintf(level_text, sizeof(level_text), "LEVEL %u", (unsigned)scene->play.level_id);
    if (scene_draw_text_colored(scene, level_text, (int16_t)(progress_x),
                                (int16_t)(progress_y - 12),
                                BODY_SCALE, COLOR_TEXT_DIM) != 0)
      return 1;

    if (total_waves == 0)
      display_wave = 0;
    else if (display_wave > total_waves)
      display_wave = total_waves;

    char round_text[16];
    snprintf(round_text, sizeof(round_text), "%u/%u",
             (unsigned)display_wave, (unsigned)total_waves);
    const uint16_t round_text_width = text_utils_estimate_width(round_text, BODY_SCALE);
    const int16_t round_text_x = (int16_t)(card_x + card_width - 15 - round_text_width);
    if (scene_draw_text_colored(scene, round_text, round_text_x,
                                (int16_t)(progress_y - 12),
                                BODY_SCALE, COLOR_TEXT_DARK) != 0)
      return 1;

    uint16_t completed_waves = scene->play.wave_ctrl.current_wave;
    if (completed_waves > total_waves)
      completed_waves = total_waves;
    const uint16_t progress_fill_w = (total_waves == 0) ? 0 :
        (uint16_t)(((uint32_t)completed_waves * progress_inner_w) / total_waves);

    if (scene_fill_frame(progress_x, progress_y, progress_w, progress_h,
                         COLOR_TEXT_DARK, COLOR_CARD_INSET) != 0)
      return 1;
    if (progress_fill_w > 0)
    {
      if (vg_fill_rect_to_buffer((uint16_t)(progress_x + 2), (uint16_t)(progress_y + 2),
                                   progress_fill_w, (uint16_t)(progress_h - 4),
                                   COLOR_GRASS_LIGHT) != 0)
        return 1;
    }
    return 0;
  }

  const uint16_t sidebar_x = MAP_WIDTH;
  const uint16_t card_x = SIDEBAR_CARD_X;
  const uint16_t card_width = SIDEBAR_CARD_WIDTH;
  const uint16_t shop_y = SIDEBAR_SHOP_Y;
  const uint16_t shop_height = SIDEBAR_SHOP_HEIGHT;
  const uint16_t context_y = SIDEBAR_CONTEXT_Y;
  const uint16_t context_height = SIDEBAR_CONTEXT_HEIGHT;
  const uint16_t wave_y = SIDEBAR_WAVE_Y;
  const uint16_t wave_height = (uint16_t)(get_v_res() - wave_y - 8);

  PROFILE_BEGIN("sidebar_bg");
  PROFILE_BEGIN("sidebar_bg_fill");
  if (vg_fill_rect_to_buffer(sidebar_x, 0, SIDEBAR_WIDTH, (uint16_t)get_v_res(), COLOR_SIDEBAR_BG) != 0)
  {
    PROFILE_END();
    PROFILE_END();
    return 1;
  }
  PROFILE_END();

  PROFILE_BEGIN("sidebar_divider");
  if (vg_fill_rect_to_buffer(sidebar_x, 0, 4, (uint16_t)get_v_res(), COLOR_TEXT_DARK) != 0)
  {
    PROFILE_END();
    PROFILE_END();
    return 1;
  }
  PROFILE_END();
  PROFILE_END();

  PROFILE_BEGIN("sidebar_panels");
  if (panel_draw(card_x, shop_y, card_width, shop_height) != 0)
  {
    PROFILE_END();
    return 1;
  }
  PROFILE_END();

  PROFILE_BEGIN("sidebar_shop");
  if (scene_draw_centered_text_colored(scene, "SHOP", card_x, 23, card_width,
                                       HUD_SCALE, COLOR_TEXT_DARK) != 0)
  {
    PROFILE_END();
    return 1;
  }

  if (scene_draw_tower_shop_card(scene, TOWER_KIND_BASIC, scene_get_tower_shop_sprite(scene, TOWER_KIND_BASIC), (uint16_t)(card_x + 12), 46, "NORMAL") != 0)
  {
    PROFILE_END();
    return 1;
  }

  if (scene_draw_tower_shop_card(scene, TOWER_KIND_GUN, scene_get_tower_shop_sprite(scene, TOWER_KIND_GUN), (uint16_t)(card_x + 88), 46, "SNIPER") != 0)
  {
    PROFILE_END();
    return 1;
  }

  if (scene_draw_tower_shop_card(scene, TOWER_KIND_SMOKE, scene_get_tower_shop_sprite(scene, TOWER_KIND_SMOKE), (uint16_t)(card_x + 12), 128, "SMOKE") != 0)
  {
    PROFILE_END();
    return 1;
  }

  if (scene_draw_tower_shop_card(scene, TOWER_KIND_POISON, scene_get_tower_shop_sprite(scene, TOWER_KIND_POISON), (uint16_t)(card_x + 88), 128, "POISON") != 0)
  {
    PROFILE_END();
    return 1;
  }
  PROFILE_END();

  PROFILE_BEGIN("sidebar_context");
  if (scene->play.selected_tower >= 0)
  {
    if (scene_draw_tower_context_panel(scene, card_x, context_y, card_width, context_height,
                                        (uint16_t)scene->play.selected_tower) != 0)
    {
      PROFILE_END();
      return 1;
    }
  }
  else
  {
    if (scene_draw_empty_context_panel(scene, card_x, context_y, card_width, context_height) != 0)
    {
      PROFILE_END();
      return 1;
    }
  }
  PROFILE_END();

  PROFILE_BEGIN("sidebar_wave");
  const bool defender_mode = (scene->multiplayer_role == MULTIPLAYER_ROLE_DEFENDER);
  if (!scene->wave_panel_static_cached ||
      scene->wave_panel_static_level_id != scene->play.level_id ||
      scene->wave_panel_static_defender != defender_mode)
  {
    if (scene_build_wave_panel_static(scene, card_x, wave_y, card_width, wave_height) != 0)
    {
      PROFILE_END();
      return 1;
    }
  }

  if (vg_opaque_sprite_to_buffer(&scene->wave_panel_static, card_x, wave_y) != 0)
  {
    PROFILE_END();
    return 1;
  }

  uint16_t total_waves = scene->play.wave_ctrl.wave_count;
  uint16_t display_wave = (uint16_t)(scene->play.wave_ctrl.current_wave + 1);
  if (total_waves == 0)
    display_wave = 0;
  else if (display_wave > total_waves)
    display_wave = total_waves;

  char round_text[16];
  snprintf(round_text, sizeof(round_text), "%u/%u",
           (unsigned)display_wave, (unsigned)total_waves);
  const uint16_t round_text_width = text_utils_estimate_width(round_text, BODY_SCALE);
  const int16_t round_text_x = (int16_t)(card_x + card_width - 15 - round_text_width);

  if (scene_draw_text_colored(scene, round_text, round_text_x,
                               (int16_t)(wave_y + 16),
                               BODY_SCALE, COLOR_TEXT_DARK) != 0)
  {
    PROFILE_END();
    return 1;
  }

  const uint16_t progress_x = (uint16_t)(card_x + 16);
  const uint16_t progress_y = (uint16_t)(wave_y + 32);
  const uint16_t progress_w = 140;
  const uint16_t progress_h = 8;
  const uint16_t progress_inner_w = (uint16_t)(progress_w - 4);
  uint16_t completed_waves = scene->play.wave_ctrl.current_wave;
  if (completed_waves > total_waves)
    completed_waves = total_waves;
  const uint16_t progress_fill_w = (total_waves == 0) ? 0 :
      (uint16_t)(((uint32_t)completed_waves * progress_inner_w) / total_waves);

  if (progress_fill_w > 0)
  {
    if (vg_fill_rect_to_buffer((uint16_t)(progress_x + 2), (uint16_t)(progress_y + 2),
                                progress_fill_w, (uint16_t)(progress_h - 4),
                                COLOR_GRASS_LIGHT) != 0)
    {
      PROFILE_END();
      return 1;
    }
  }
  if (defender_mode){
    const uint16_t active_bar_y = (uint16_t)(progress_y + 32);
    const uint32_t active_count = (uint32_t)scene->play.wave_ctrl.enemies.active_count;
    const uint32_t active_max   = (uint32_t)play_state_multiplayer_wave_remaining_total(&scene->play) + active_count;
    const uint16_t active_fill_w = (active_max == 0) ? 0 :
        (uint16_t)((active_count * progress_inner_w) / active_max);

    char active_val[16];
    snprintf(active_val, sizeof(active_val), "%u", (unsigned)active_count);
    uint16_t val_w = text_utils_estimate_width(active_val, BODY_SCALE);
    if (scene_draw_text_colored(scene, active_val,
                                (int16_t)(card_x + card_width - 15 - val_w),
                                (int16_t)(active_bar_y - 12),
                                BODY_SCALE, COLOR_TEXT_DARK) != 0)
    {
      PROFILE_END();
      return 1;
    }
    if (active_fill_w > 0 &&
        vg_fill_rect_to_buffer((uint16_t)(progress_x + 2), (uint16_t)(active_bar_y + 2),
                                active_fill_w, (uint16_t)(progress_h - 4),
                                COLOR_GRASS_LIGHT) != 0)
    {
      PROFILE_END();
      return 1;
    }
  }
  const char *wave_status = scene->play.wave_ctrl.level_complete ? "CLEAR" :
      scene->play.wave_ctrl.wave_active ? "ONGOING" : "NEXT";

  const uint16_t wave_btn_w = (uint16_t)(34U * UI_BUTTON_SCALE);
  const uint16_t wave_btn_h = (uint16_t)(14U * UI_BUTTON_SCALE);
  const uint16_t wave_btn_y = (uint16_t)(wave_y + 54);
  const uint16_t wave_status_x = (uint16_t)(card_x + 13);
  const uint16_t ff_btn_x = (uint16_t)(card_x + 90);

  if (scene->multiplayer_role == MULTIPLAYER_ROLE_NONE &&
      scene_draw_button(scene,
                         scene_get_sidebar_button_sprite(scene, true, wave_status_x,
                                                         wave_btn_y, wave_btn_w,
                                                         wave_btn_h),
                         wave_status_x, wave_btn_y,
                         wave_status, COLOR_TEXT_SOFT) != 0)
  {
    PROFILE_END();
    return 1;
  }

  const char *ff_label = (scene->play.speed_multiplier > 1) ? "2X" : "1X";
  if (scene->multiplayer_role == MULTIPLAYER_ROLE_NONE &&
      scene_draw_button(scene,
                         scene_get_sidebar_button_sprite(scene, false, ff_btn_x,
                                                         wave_btn_y, wave_btn_w,
                                                         wave_btn_h),
                         ff_btn_x, wave_btn_y,
                         ff_label, COLOR_TEXT_DARK) != 0)
  {
    PROFILE_END();
    return 1;
  }
  PROFILE_END();

  return 0;
}
