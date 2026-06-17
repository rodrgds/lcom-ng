/**
 * @file button.h
 * @brief Reusable sidebar button sprite selection and drawing.
 * @ingroup group_ui
 */

#ifndef PROJ_BUTTON_H
#define PROJ_BUTTON_H

#include <stdbool.h>
#include <stdint.h>

#include "video_gr_ext.h"

struct scene_t;

const vg_sprite_t *scene_get_sidebar_button_sprite(const struct scene_t *scene,
  bool green, uint16_t x, uint16_t y, uint16_t w, uint16_t h);

int scene_draw_button(struct scene_t *scene, const vg_sprite_t *button_sprite,
  uint16_t x, uint16_t y, const char *label, uint32_t color);

#endif
