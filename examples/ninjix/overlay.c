#include "overlay.h"

#include <stdio.h>

#include "video_gr_ext.h"
#include "scene.h"
#include "button.h"
#include "colors.h"
#include "overlay_menu.h"
#include "panel.h"
#include "pause_menu.h"

static int scene_draw_overlay_buttons(scene_t *scene, const overlay_menu_t *menu,
                                const char *const *labels)
{
  if (scene == NULL || menu == NULL || labels == NULL)
    return 1;

  for (uint8_t i = 0; i < menu->option_count; i++)
  {
    uint16_t x;
    uint16_t y;
    uint16_t width;
    uint16_t height;
    (void)width;
    (void)height;
    overlay_menu_get_option_bounds(menu, i, &x, &y, &width, &height);

    const bool mouse_over = menu->hovered_option == (int16_t)i;
    const bool keyboard_selected = menu->hovered_option < 0 &&
                                    menu->selected_option == (int16_t)i;
    const vg_sprite_t *button;
    uint32_t color;

    if (mouse_over)
    {
      button = scene_get_sidebar_button_sprite(scene, true, x, y, width, height);
      color = COLOR_TEXT_SOFT;
    }
    else if (keyboard_selected)
    {
      button = &scene->resources->button_green_sprite;
      color = COLOR_TEXT_SOFT;
    }
    else
    {
      button = &scene->resources->button_silver_sprite;
      color = COLOR_TEXT_DARK;
    }

    if (button == NULL)
      return 1;

    if (scene_draw_button(scene, button, x, y - 4, labels[i], color) != 0)
      return 1;
  }

  return 0;
}

int scene_draw_pause_overlay_with_status(scene_t *scene, const pause_menu_t *pause,
                                           const char *status_text)
{
  const uint16_t panel_w = 260;
  const uint16_t panel_h = (status_text == NULL) ? 90 : 124;
  const uint16_t panel_x = (uint16_t)((get_h_res() - panel_w) / 2U);
  const uint16_t panel_y = (uint16_t)((get_v_res() - panel_h) / 2U);
  static const char *const pause_labels[] = {"RESUME", "CLOSE"};

  if (panel_draw(panel_x, panel_y - 18, panel_w, panel_h) != 0)
    return 1;

  if (scene_draw_centered_text_colored(scene, "PAUSED", panel_x,
                                       (uint16_t)(panel_y + 18),
                                       panel_w, HUD_SCALE, COLOR_TEXT_DARK) != 0)
    return 1;

  if (status_text != NULL &&
      scene_draw_centered_text_colored(scene, status_text, panel_x,
                                       (uint16_t)(panel_y + 44),
                                       panel_w, BODY_SCALE, COLOR_TEXT_DIM) != 0)
    return 1;

  if (pause == NULL)
    return 0;

  return scene_draw_overlay_buttons(scene, &pause->menu, pause_labels);
}

int scene_draw_pause_overlay(scene_t *scene, const pause_menu_t *pause)
{
  return scene_draw_pause_overlay_with_status(scene, pause, NULL);
}

int scene_draw_result_overlay(scene_t *scene, const char *title,
                               const overlay_menu_t *menu,
                               const char *const *button_labels,
                               const char *status_text)
{
  const uint16_t panel_w = 300;
  const uint16_t panel_h = 128;
  const uint16_t panel_x = (uint16_t)((get_h_res() - panel_w) / 2U);
  const uint16_t panel_y = (uint16_t)((get_v_res() - panel_h) / 2U);

  if (panel_draw(panel_x, panel_y, panel_w, panel_h) != 0)
    return 1;

  if (scene_draw_centered_text_colored(scene, title, panel_x,
                                       (uint16_t)(panel_y + 18),
                                       panel_w, HUD_SCALE, COLOR_TEXT_DARK) != 0)
    return 1;

  char stats_text[40];
  snprintf(stats_text, sizeof(stats_text), "KILLS %u  COINS %u",
           (unsigned)scene->play.enemies_killed,
           (unsigned)scene->play.coins);

  if (scene_draw_centered_text_colored(scene, stats_text, panel_x,
                                       (uint16_t)(panel_y + 52),
                                       panel_w, BODY_SCALE, COLOR_TEXT_DIM) != 0)
    return 1;

  if (status_text != NULL &&
      scene_draw_centered_text_colored(scene, status_text, panel_x,
                                       (uint16_t)(panel_y + 70),
                                       panel_w, BODY_SCALE, COLOR_TEXT_DIM) != 0)
    return 1;

  return scene_draw_overlay_buttons(scene, menu, button_labels);
}
