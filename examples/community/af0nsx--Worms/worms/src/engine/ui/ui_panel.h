#ifndef UI_PANEL_H
#define UI_PANEL_H

#include <stdint.h>
#include <stdbool.h>
#include "../graphics/font.h"

/**
 * Geometry and colours for a bordered rectangle.
 * Declare instances as `static const UiPanel` in module scope.
 */
typedef struct {
  int      x, y, w, h;
  int      border;
  uint32_t border_color;
  uint32_t fill_color;
  uint32_t fill_selected;  /* fill used when selected == true */
} UiPanel;

/**
 * @brief Draws the panel, optionally with a centred font label.
 *        When selected is true, fill_selected replaces fill_color.
 *
 * @param p       Panel to draw. NULL is a no-op returning 1.
 * @param f       Font for the label. NULL skips text.
 * @param label   Null-terminated string to centre. NULL skips text.
 * @param selected Whether to use fill_selected instead of fill_color.
 * @return 0 on success, 1 on any draw error.
 */
int ui_panel_draw(const UiPanel *p, const Font *f,
                  const char *label, bool selected);

/**
 * @brief Returns true if (px, py) lies inside the panel bounding box.
 */
bool ui_panel_contains(const UiPanel *p, int px, int py);

#endif /* UI_PANEL_H */
