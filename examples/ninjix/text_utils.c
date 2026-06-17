/**
 * @file text_utils.c
 * @brief Text width estimation for proportional HUD font.
 * @ingroup group_ui
 */

#include "text_utils.h"

uint16_t text_utils_estimate_width(const char *text, uint16_t scale)
{
  uint16_t width = 0;
  for (const char *p = text; *p != '\0'; p++)
    width = (uint16_t)(width + (6U * scale));
  return width;
}
