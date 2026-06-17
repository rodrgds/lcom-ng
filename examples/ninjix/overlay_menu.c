/**
 * @file overlay_menu.c
 * @brief Overlay menu implementation -- generic button bar for pause and result screens.
 * @ingroup group_ui
 */

#include "overlay_menu.h"

#include <stddef.h>

#include "scancodes.h"

void overlay_menu_init(overlay_menu_t *menu, uint8_t option_count,
                       uint16_t button_x, uint16_t button_y,
                       uint16_t button_width, uint16_t button_height,
                       uint16_t button_gap)
{
  if (menu == NULL)
    return;

  menu->selected_option = 0;
  menu->hovered_option = -1;
  menu->option_count = option_count;
  menu->button_x = button_x;
  menu->button_y = button_y;
  menu->button_width = button_width;
  menu->button_height = button_height;
  menu->button_gap = button_gap;
}

int16_t overlay_menu_handle_keyboard(overlay_menu_t *menu, bool make, uint8_t scancode)
{
  if (menu == NULL || !make || menu->option_count == 0)
    return -1;

  switch (scancode)
  {
    case SCANCODE_UP:
    case SCANCODE_LEFT:
    case SCANCODE_W:
    case SCANCODE_A:
      menu->selected_option--;
      if (menu->selected_option < 0)
        menu->selected_option = (int16_t)(menu->option_count - 1);
      menu->hovered_option = -1;
      return -1;

    case SCANCODE_DOWN:
    case SCANCODE_RIGHT:
    case SCANCODE_S:
    case SCANCODE_D:
      menu->selected_option++;
      if (menu->selected_option >= (int16_t)menu->option_count)
        menu->selected_option = 0;
      menu->hovered_option = -1;
      return -1;

    case SCANCODE_ENTER:
      return menu->selected_option;

    default:
      return -1;
  }
}

void overlay_menu_get_option_bounds(const overlay_menu_t *menu, uint8_t option,
                                    uint16_t *left, uint16_t *top,
                                    uint16_t *width, uint16_t *height)
{
  if (menu == NULL)
    return;

  if (left != NULL)
    *left = (uint16_t)(menu->button_x + option * (menu->button_width + menu->button_gap));
  if (top != NULL)
    *top = menu->button_y;
  if (width != NULL)
    *width = menu->button_width;
  if (height != NULL)
    *height = menu->button_height;
}

void overlay_menu_handle_mouse_move(overlay_menu_t *menu, int16_t cursor_x, int16_t cursor_y)
{
  if (menu == NULL)
    return;

  menu->hovered_option = -1;

  for (uint8_t i = 0; i < menu->option_count; i++)
  {
    uint16_t left;
    uint16_t top;
    uint16_t width;
    uint16_t height;
    overlay_menu_get_option_bounds(menu, i, &left, &top, &width, &height);

    if (cursor_x >= (int16_t)left && cursor_x <= (int16_t)(left + width) &&
        cursor_y >= (int16_t)top && cursor_y <= (int16_t)(top + height))
    {
      menu->hovered_option = i;
      menu->selected_option = i;
      return;
    }
  }
}

int16_t overlay_menu_handle_mouse_click(const overlay_menu_t *menu)
{
  if (menu == NULL || menu->hovered_option < 0)
    return -1;

  return menu->hovered_option;
}
