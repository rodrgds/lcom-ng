/**
 * @file overlay_menu.h
 * @brief Reusable overlay menu -- option selection, bounds computation, and input.
 * @ingroup group_ui
 *
 * A generic horizontal button bar used by both the pause menu and the
 * victory / game-over result screen. Handles keyboard (up/down/enter) and
 * mouse hit-testing for a configurable number of options.
 */

#ifndef PROJ_OVERLAY_MENU_H
#define PROJ_OVERLAY_MENU_H

#include <stdbool.h>
#include <stdint.h>

typedef struct {
  int16_t selected_option;
  int16_t hovered_option;
  uint8_t option_count;
  uint16_t button_x;
  uint16_t button_y;
  uint16_t button_width;
  uint16_t button_height;
  uint16_t button_gap;
} overlay_menu_t;

void overlay_menu_init(overlay_menu_t *menu, uint8_t option_count,
                       uint16_t button_x, uint16_t button_y,
                       uint16_t button_width, uint16_t button_height,
                       uint16_t button_gap);
int16_t overlay_menu_handle_keyboard(overlay_menu_t *menu, bool make, uint8_t scancode);
void overlay_menu_handle_mouse_move(overlay_menu_t *menu, int16_t cursor_x, int16_t cursor_y);
int16_t overlay_menu_handle_mouse_click(const overlay_menu_t *menu);
void overlay_menu_get_option_bounds(const overlay_menu_t *menu, uint8_t option,
                                    uint16_t *left, uint16_t *top,
                                    uint16_t *width, uint16_t *height);

#endif
