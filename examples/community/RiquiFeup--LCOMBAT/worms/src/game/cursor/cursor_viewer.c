#include "cursor_viewer.h"

#include <stddef.h>

#include "../../assets/assets.h"
#include "../../engine/graphics/renderer.h"

void cursor_viewer_draw(const Cursor *cursor) {
  if (cursor == NULL) return;

  renderer_draw_sprite(assets_get_cursor(), cursor_get_x(cursor), cursor_get_y(cursor));
}
