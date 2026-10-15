#include "cursor_controller.h"

#include <stddef.h>

#include "../../config.h"

static int16_t clamp_coord(int16_t value, int16_t min, int16_t max) {
  if (value < min) return min;
  if (value > max) return max;
  return value;
}

void cursor_controller_handle_event(Cursor *cursor, const GameEvent *event) {
  if (cursor == NULL || event == NULL) return;
  if (event->type != EVENT_MOUSE_MOVE && event->type != EVENT_MOUSE_BTN) return;

  int16_t x = clamp_coord(event->data.mouse.x, 0, SCREEN_W - 1);
  int16_t y = clamp_coord(event->data.mouse.y, 0, SCREEN_H - 1);

  cursor_set_position(cursor, x, y);
  cursor_set_left_pressed(cursor, event->data.mouse.lb);
}
