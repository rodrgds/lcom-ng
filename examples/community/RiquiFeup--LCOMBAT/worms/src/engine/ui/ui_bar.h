#ifndef UI_BAR_H
#define UI_BAR_H

#include <stdint.h>

/**
 * Geometry and colours for a horizontal fill bar (health, timer, etc.).
 */
typedef struct {
  int      x;
  int      y;
  int      w;
  int      h;
  int      border;
  uint32_t border_color;
  uint32_t bg_color;    /* empty section colour */
  uint32_t fill_ok;     /* fill when value/max >= low_pct */
  uint32_t fill_low;    /* fill when value/max <  low_pct */
  uint8_t  low_pct;     /* threshold in percent (0-100) */
} UiBar;

/**
 * @brief Draws the bar filled to value/max_value fraction.
 *        Colour switches to fill_low when (value*100/max_value) < low_pct.
 *
 * @param bar       Bar descriptor. NULL is a no-op.
 * @param value     Current value.
 * @param max_value Maximum value. 0 is a no-op.
 */
void ui_bar_draw(const UiBar *bar, uint16_t value, uint16_t max_value);

#endif /* UI_BAR_H */
