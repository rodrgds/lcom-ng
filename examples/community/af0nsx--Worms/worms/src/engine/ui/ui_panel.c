#include "ui_panel.h"

#include "../graphics/renderer.h"
#include "../graphics/font.h"
#include "config.h"

int ui_panel_draw(const UiPanel *p, const Font *f,
                  const char *label, bool selected) {
  if (p == NULL) return 1;
  uint32_t fill = selected ? p->fill_selected : p->fill_color;
  if (renderer_draw_rect(p->x, p->y, p->w, p->h, p->border_color) != 0) return 1;

  if (renderer_draw_rect(p->x + p->border, p->y + p->border,
                         p->w - 2 * p->border, p->h - 2 * p->border,
                         fill) != 0) return 1;

  if (f != NULL && label != NULL) {
    int tw = font_measure_string(f, label);
    int tx = p->x + (p->w - tw) / 2;
    int ty = p->y + (p->h - FONT_GLYPH_HEIGHT) / 2;
    font_draw_string(f, label, tx, ty);
  }
  return 0;
}

bool ui_panel_contains(const UiPanel *p, int px, int py) {
  if (p == NULL) return false;
  return px >= p->x && px < p->x + p->w
      && py >= p->y && py < p->y + p->h;
}
