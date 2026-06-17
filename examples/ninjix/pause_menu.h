/**
 * @file pause_menu.h
 * @brief Pause overlay -- resume / close actions during gameplay pause.
 * @ingroup group_ui
 *
 * A lightweight overlay menu that appears when the game is paused.
 * Supports keyboard and mouse navigation, returning action enums
 * consumed by the app state machine.
 */

#ifndef PROJ_PAUSE_MENU_H
#define PROJ_PAUSE_MENU_H

#include <stdbool.h>
#include <stdint.h>

#include "overlay_menu.h"

typedef enum {
  PAUSE_MENU_ACTION_NONE = 0,
  PAUSE_MENU_ACTION_RESUME,
  PAUSE_MENU_ACTION_CLOSE,
} pause_menu_action_t;

typedef enum {
  PAUSE_MENU_OPTION_RESUME = 0,
  PAUSE_MENU_OPTION_CLOSE,
  PAUSE_MENU_OPTION_COUNT,
} pause_menu_option_t;

typedef struct {
  overlay_menu_t menu;
} pause_menu_t;

void pause_menu_init(pause_menu_t *pause);
pause_menu_action_t pause_menu_handle_keyboard(pause_menu_t *pause, bool make, uint8_t scancode);
void pause_menu_handle_mouse_move(pause_menu_t *pause, int16_t cursor_x, int16_t cursor_y);
pause_menu_action_t pause_menu_handle_mouse_click(const pause_menu_t *pause);

#endif
