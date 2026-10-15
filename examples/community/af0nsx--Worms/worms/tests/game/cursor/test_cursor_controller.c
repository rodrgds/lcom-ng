#include <assert.h>
#include <stdbool.h>
#include <stddef.h>

#include "../../../src/config.h"
#include "../../../src/game/cursor/cursor.h"
#include "../../../src/game/cursor/cursor_controller.h"

static GameEvent mouse_event(int16_t x, int16_t y, bool lb) {
  GameEvent event;

  event.type = EVENT_MOUSE_MOVE;
  event.data.mouse.x = x;
  event.data.mouse.y = y;
  event.data.mouse.lb = lb;
  event.data.mouse.rb = false;

  return event;
}

static void test_mouse_move_updates_position(void) {
  Cursor *cursor = cursor_create();
  GameEvent event = mouse_event(42, 84, false);

  assert(cursor != NULL);

  cursor_controller_handle_event(cursor, &event);

  assert(cursor_get_x(cursor) == 42);
  assert(cursor_get_y(cursor) == 84);
  assert(!cursor_is_left_pressed(cursor));

  cursor_destroy(cursor);
}

static void test_left_click_edge_only_on_press(void) {
  Cursor *cursor = cursor_create();
  GameEvent pressed = mouse_event(10, 20, true);

  assert(cursor != NULL);

  cursor_controller_handle_event(cursor, &pressed);
  assert(cursor_is_left_pressed(cursor));
  assert(cursor_was_left_clicked(cursor));
  assert(cursor_consume_left_click(cursor));
  assert(!cursor_was_left_clicked(cursor));
  assert(!cursor_consume_left_click(cursor));

  cursor_controller_handle_event(cursor, &pressed);
  assert(cursor_is_left_pressed(cursor));
  assert(!cursor_was_left_clicked(cursor));
  assert(!cursor_consume_left_click(cursor));

  pressed.data.mouse.lb = false;
  cursor_controller_handle_event(cursor, &pressed);
  assert(!cursor_is_left_pressed(cursor));
  assert(!cursor_was_left_clicked(cursor));
  assert(!cursor_consume_left_click(cursor));

  cursor_destroy(cursor);
}

static void test_release_then_press_creates_new_consumable_click(void) {
  Cursor *cursor = cursor_create();
  GameEvent event = mouse_event(10, 20, true);

  assert(cursor != NULL);

  cursor_controller_handle_event(cursor, &event);
  assert(cursor_consume_left_click(cursor));

  event.data.mouse.lb = false;
  cursor_controller_handle_event(cursor, &event);
  assert(!cursor_consume_left_click(cursor));

  event.data.mouse.lb = true;
  cursor_controller_handle_event(cursor, &event);
  assert(cursor_was_left_clicked(cursor));
  assert(cursor_consume_left_click(cursor));
  assert(!cursor_consume_left_click(cursor));

  cursor_destroy(cursor);
}

static void test_non_mouse_event_is_ignored(void) {
  Cursor *cursor = cursor_create();
  GameEvent event;

  assert(cursor != NULL);

  event.type = EVENT_TICK;
  cursor_controller_handle_event(cursor, &event);

  assert(cursor_get_x(cursor) == SCREEN_W / 2);
  assert(cursor_get_y(cursor) == SCREEN_H / 2);

  cursor_destroy(cursor);
}

static void test_position_is_clamped(void) {
  Cursor *cursor = cursor_create();
  GameEvent event = mouse_event(-10, SCREEN_H + 10, false);

  assert(cursor != NULL);

  cursor_controller_handle_event(cursor, &event);

  assert(cursor_get_x(cursor) == 0);
  assert(cursor_get_y(cursor) == SCREEN_H - 1);

  cursor_destroy(cursor);
}

int main(void) {
  test_mouse_move_updates_position();
  test_left_click_edge_only_on_press();
  test_release_then_press_creates_new_consumable_click();
  test_non_mouse_event_is_ignored();
  test_position_is_clamped();

  return 0;
}
