#include <assert.h>
#include <stdint.h>

#include "../../../src/game/cursor/cursor.h"
#include "../../../src/game/cursor/cursor_viewer.h"
#include "../../../src/engine/graphics/sprite.h"

static int renderer_draw_sprite_calls = 0;
static const Sprite *last_sprite = 0;
static int last_x = 0;
static int last_y = 0;

const Sprite *assets_get_cursor(void) {
  return (const Sprite*) 1;
}

int renderer_draw_sprite(const Sprite *sprite, int x, int y) {
  renderer_draw_sprite_calls++;
  last_sprite = sprite;
  last_x = x;
  last_y = y;
  return 0;
}

static void test_draw_uses_cursor_position(void) {
  Cursor *cursor = cursor_create();

  assert(cursor != 0);
  cursor_set_position(cursor, 12, 34);

  renderer_draw_sprite_calls = 0;
  last_sprite = 0;
  last_x = 0;
  last_y = 0;

  cursor_viewer_draw(cursor);

  assert(renderer_draw_sprite_calls == 1);
  assert(last_sprite == assets_get_cursor());
  assert(last_x == 12);
  assert(last_y == 34);

  cursor_destroy(cursor);
}

static void test_draw_ignores_null_cursor(void) {
  renderer_draw_sprite_calls = 0;

  cursor_viewer_draw(0);

  assert(renderer_draw_sprite_calls == 0);
}

int main(void) {
  test_draw_uses_cursor_position();
  test_draw_ignores_null_cursor();

  return 0;
}
