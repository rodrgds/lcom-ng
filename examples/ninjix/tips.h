/**
 * @file tips.h
 * @brief Tip-of-the-day panel with icons for the sidebar.
 * @ingroup group_ui
 */

#ifndef PROJ_TIPS_H
#define PROJ_TIPS_H

#include <stdbool.h>
#include <stdint.h>

struct scene_t;

int scene_draw_tips_panel(struct scene_t *scene,
  uint16_t x, uint16_t y,
  uint16_t width, uint16_t height, bool isAttackingMultiplayer);

#endif
