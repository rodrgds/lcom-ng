/**
 * @file panel.c
 * @brief Panel drawing implementation -- bordered region with wood/parchment theme.
 * @ingroup group_ui
 */

#include "panel.h"

#include <stdbool.h>

#include "video_gr_ext.h"
#include "colors.h"
#include "sprite_utils.h"

static int panel_fill_rect(bool to_buffer, vg_sprite_t *sprite,
                           uint16_t x, uint16_t y, uint16_t width, uint16_t height,
                           uint32_t color)
{
  if (to_buffer)
    return vg_fill_rect_to_buffer(x, y, width, height, color);
  return sprite_utils_fill_rect(sprite, x, y, width, height, color);
}

int panel_draw(uint16_t x, uint16_t y, uint16_t width, uint16_t height)
{
  return panel_draw_to_sprite(NULL, x, y, width, height);
}

int panel_draw_to_sprite(vg_sprite_t *sprite, uint16_t x, uint16_t y,
                         uint16_t width, uint16_t height)
{
  const uint16_t border = 5;
  if (width <= (uint16_t)(border * 2) || height <= (uint16_t)(border * 2))
    return 1;

  const bool to_buffer = (sprite == NULL);

  // Fill the interior with the panel background colour
  if (panel_fill_rect(to_buffer, sprite,
                      (uint16_t)(x + border), (uint16_t)(y + border),
                      (uint16_t)(width - border * 2), (uint16_t)(height - border * 2),
                      COLOR_PANEL_FILL) != 0)
    return 1;

  // Draw four 1 px border rings as the walls of the panel, each inset by one pixel,
  // going from dark (outermost) through mid and inner to accent (innermost)
  // this gives a really good "gradient" effect!
  if (panel_fill_rect(to_buffer, sprite, x, y, width, 1, COLOR_PANEL_BORDER_DARK) != 0 ||
      panel_fill_rect(to_buffer, sprite, x, (uint16_t)(y + height - 1), width, 1, COLOR_PANEL_BORDER_DARK) != 0 ||
      panel_fill_rect(to_buffer, sprite, x, (uint16_t)(y + 1), 1, (uint16_t)(height - 2), COLOR_PANEL_BORDER_DARK) != 0 ||
      panel_fill_rect(to_buffer, sprite, (uint16_t)(x + width - 1), (uint16_t)(y + 1), 1, (uint16_t)(height - 2), COLOR_PANEL_BORDER_DARK) != 0)
    return 1;

  if (panel_fill_rect(to_buffer, sprite, (uint16_t)(x + 1), (uint16_t)(y + 1), (uint16_t)(width - 2), 1, COLOR_PANEL_BORDER_MID) != 0 ||
      panel_fill_rect(to_buffer, sprite, (uint16_t)(x + 1), (uint16_t)(y + height - 2), (uint16_t)(width - 2), 1, COLOR_PANEL_BORDER_MID) != 0 ||
      panel_fill_rect(to_buffer, sprite, (uint16_t)(x + 1), (uint16_t)(y + 2), 1, (uint16_t)(height - 4), COLOR_PANEL_BORDER_MID) != 0 ||
      panel_fill_rect(to_buffer, sprite, (uint16_t)(x + width - 2), (uint16_t)(y + 2), 1, (uint16_t)(height - 4), COLOR_PANEL_BORDER_MID) != 0)
    return 1;

  if (panel_fill_rect(to_buffer, sprite, (uint16_t)(x + 2), (uint16_t)(y + 2), (uint16_t)(width - 4), 1, COLOR_PANEL_BORDER_INNER) != 0 ||
      panel_fill_rect(to_buffer, sprite, (uint16_t)(x + 2), (uint16_t)(y + height - 3), (uint16_t)(width - 4), 1, COLOR_PANEL_BORDER_INNER) != 0 ||
      panel_fill_rect(to_buffer, sprite, (uint16_t)(x + 2), (uint16_t)(y + 3), 1, (uint16_t)(height - 6), COLOR_PANEL_BORDER_INNER) != 0 ||
      panel_fill_rect(to_buffer, sprite, (uint16_t)(x + width - 3), (uint16_t)(y + 3), 1, (uint16_t)(height - 6), COLOR_PANEL_BORDER_INNER) != 0)
    return 1;

  if (panel_fill_rect(to_buffer, sprite, (uint16_t)(x + 3), (uint16_t)(y + 3), (uint16_t)(width - 6), 1, COLOR_PANEL_BORDER_ACCENT) != 0 ||
      panel_fill_rect(to_buffer, sprite, (uint16_t)(x + 3), (uint16_t)(y + height - 4), (uint16_t)(width - 6), 1, COLOR_PANEL_BORDER_ACCENT) != 0 ||
      panel_fill_rect(to_buffer, sprite, (uint16_t)(x + 3), (uint16_t)(y + 4), 1, (uint16_t)(height - 8), COLOR_PANEL_BORDER_ACCENT) != 0 ||
      panel_fill_rect(to_buffer, sprite, (uint16_t)(x + width - 4), (uint16_t)(y + 4), 1, (uint16_t)(height - 8), COLOR_PANEL_BORDER_ACCENT) != 0)
    return 1;

  // 2x2 inside corner dots
  const uint16_t corner_size = 2;
  if (panel_fill_rect(to_buffer, sprite, (uint16_t)(x + border), (uint16_t)(y + border),
                      corner_size, corner_size, COLOR_PANEL_BORDER_DARK) != 0 ||
      panel_fill_rect(to_buffer, sprite, (uint16_t)(x + width - border - corner_size), (uint16_t)(y + border),
                      corner_size, corner_size, COLOR_PANEL_BORDER_DARK) != 0 ||
      panel_fill_rect(to_buffer, sprite, (uint16_t)(x + border), (uint16_t)(y + height - border - corner_size),
                      corner_size, corner_size, COLOR_PANEL_BORDER_DARK) != 0 ||
      panel_fill_rect(to_buffer, sprite, (uint16_t)(x + width - border - corner_size), (uint16_t)(y + height - border - corner_size),
                      corner_size, corner_size, COLOR_PANEL_BORDER_DARK) != 0)
    return 1;

  return 0;
}
