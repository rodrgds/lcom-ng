/**
 * @file pause_menu.c
 * @brief Pause menu implementation -- resume/close overlay during gameplay pause.
 * @ingroup group_ui
 */

#include "pause_menu.h"

#include <stddef.h>

#include "layout.h"

void pause_menu_init(pause_menu_t *pause)
{
  if (pause == NULL)
    return;

  overlay_menu_init(&pause->menu, PAUSE_MENU_OPTION_COUNT,
                    PAUSE_BUTTON_X, PAUSE_BUTTON_Y,
                    PAUSE_BUTTON_WIDTH, PAUSE_BUTTON_HEIGHT,
                    PAUSE_BUTTON_GAP);
}

static pause_menu_action_t pause_menu_action_for_option(int16_t option)
{
  switch (option)
  {
    case PAUSE_MENU_OPTION_RESUME:
      return PAUSE_MENU_ACTION_RESUME;
    case PAUSE_MENU_OPTION_CLOSE:
      return PAUSE_MENU_ACTION_CLOSE;
    default:
      return PAUSE_MENU_ACTION_NONE;
  }
}

pause_menu_action_t pause_menu_handle_keyboard(pause_menu_t *pause, bool make, uint8_t scancode)
{
  if (pause == NULL)
    return PAUSE_MENU_ACTION_NONE;

  return pause_menu_action_for_option(
      overlay_menu_handle_keyboard(&pause->menu, make, scancode));
}

void pause_menu_handle_mouse_move(pause_menu_t *pause, int16_t cursor_x, int16_t cursor_y)
{
  if (pause == NULL)
    return;

  overlay_menu_handle_mouse_move(&pause->menu, cursor_x, cursor_y);
}

pause_menu_action_t pause_menu_handle_mouse_click(const pause_menu_t *pause)
{
  if (pause == NULL)
    return PAUSE_MENU_ACTION_NONE;

  return pause_menu_action_for_option(overlay_menu_handle_mouse_click(&pause->menu));
}
