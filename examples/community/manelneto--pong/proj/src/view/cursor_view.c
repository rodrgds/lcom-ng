#include <lcom/lcf.h>

#include "cursor_view.h"

#include "sprite_view.h"

int draw_cursor(Cursor *cursor) {
  if (!cursor) {
    printf("%s: cursor is NULL\n", __func__);
    return 1;
  }

  if (draw_sprite(cursor->sprite, cursor->x, cursor->y)) {
    printf("%s: draw_sprite(cursor->sprite, cursor->x: %d, cursor->y: %d) error\n", __func__, cursor->x, cursor->y);
    return 1;
  }

  return 0;
}
