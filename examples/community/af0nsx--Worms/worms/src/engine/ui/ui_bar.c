#include "ui_bar.h"

#include "../graphics/renderer.h"

void ui_bar_draw(const UiBar *bar, uint16_t value, uint16_t max_value) {
  if (bar == NULL || max_value == 0) return;

  renderer_draw_rect(bar->x, bar->y, bar->w, bar->h, bar->border_color);

  int inner_x = bar->x + bar->border;
  int inner_y = bar->y + bar->border;
  int inner_w = bar->w - 2 * bar->border;
  int inner_h = bar->h - 2 * bar->border;

  if (inner_w <= 0 || inner_h <= 0) return;

  renderer_draw_rect(inner_x, inner_y, inner_w, inner_h, bar->bg_color);

  int fill_w = (inner_w * (int)value) / (int)max_value;
  if (fill_w <= 0) return;

  uint8_t pct = (uint8_t)((uint32_t)value * 100u / (uint32_t)max_value);
  uint32_t fill = (pct < bar->low_pct) ? bar->fill_low : bar->fill_ok;

  renderer_draw_rect(inner_x, inner_y, fill_w, inner_h, fill);
}
