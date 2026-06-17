/**
 * @file overlay.h
 * @brief Pause and result overlays for the gameplay screen.
 * @ingroup group_ui
 */

#ifndef PROJ_OVERLAY_H
#define PROJ_OVERLAY_H

#include <stdint.h>

#include "overlay_menu.h"
#include "pause_menu.h"

struct scene_t;

int scene_draw_pause_overlay(struct scene_t *scene, const pause_menu_t *pause);

int scene_draw_pause_overlay_with_status(struct scene_t *scene,
  const pause_menu_t *pause, const char *status_text);

int scene_draw_result_overlay(struct scene_t *scene, const char *title,
  const overlay_menu_t *menu,
  const char *const *button_labels,
  const char *status_text);

#endif
