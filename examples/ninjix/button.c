#include "button.h"

#include "video_gr_ext.h"
#include "scene.h"
#include "text_utils.h"

const vg_sprite_t *scene_get_sidebar_button_sprite(const scene_t *scene,
  bool green, uint16_t x, uint16_t y, uint16_t w, uint16_t h)
{
  bool hovered;

  if (scene == NULL || scene->resources == NULL)
    return NULL;

  hovered = scene_point_in_rect(scene->cursor_x, scene->cursor_y, x, y, w, h);

  if (green)
    return hovered ? &scene->resources->button_green_hover_sprite :
                     &scene->resources->button_green_sprite;
  return hovered ? &scene->resources->button_silver_hover_sprite :
                   &scene->resources->button_silver_sprite;
}

int scene_draw_button(scene_t *scene, const vg_sprite_t *button_sprite,
                      uint16_t x, uint16_t y, const char *label, uint32_t color)
{
  const uint16_t button_width = (uint16_t)(button_sprite->width * UI_BUTTON_SCALE);
  const uint16_t button_height = (uint16_t)(button_sprite->height * UI_BUTTON_SCALE);
  if ((uint32_t)x + button_width > get_h_res() ||
      (uint32_t)y + button_height > get_v_res())
    return 1;

  if (vg_draw_sprite_region_scaled_to_buffer(button_sprite, 0, 0,
                                             button_sprite->width, button_sprite->height,
                                             (int16_t)x, (int16_t)y, UI_BUTTON_SCALE) != 0)
    return 1;

  return scene_draw_centered_text_colored(scene, label, x, (uint16_t)(y + 8),
                                           button_width, BODY_SCALE, color);
}
