/**
 * @file context_panel.h
 * @brief Tower info panel and empty context panel for the sidebar.
 * @ingroup group_ui
 */

#ifndef PROJ_CONTEXT_PANEL_H
#define PROJ_CONTEXT_PANEL_H

#include <stdint.h>

struct scene_t;

int scene_draw_tower_context_panel(struct scene_t *scene,
  uint16_t card_x, uint16_t context_y,
  uint16_t card_width, uint16_t context_height,
  uint16_t tower_index);

int scene_draw_empty_context_panel(struct scene_t *scene,
  uint16_t card_x, uint16_t context_y,
  uint16_t card_width, uint16_t context_height);

#endif
