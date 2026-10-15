#include "cursor.h"

int cursor_init(cursor_t *cursor) {
  if (cursor == NULL)
    return 1;

  cursor->state = POINTER;
  cursor->x = vg_get_width() / 2;
  cursor->y = vg_get_height() / 2;

  cursor->pointer = sprite_create((xpm_map_t) cursor_xpm);
  if (cursor->pointer == NULL)
    return 1;

  cursor->hand = sprite_create((xpm_map_t) hand_xpm);
  if (cursor->hand == NULL) {
    sprite_destroy(cursor->pointer);
    return 1;
  }

  return 0;
}

int cursor_move(cursor_t *cursor, uint32_t x, uint32_t y) {
  if (cursor == NULL)
    return 1;

  if (x >= vg_get_width() || y >= vg_get_height())
    return 1;

  cursor->x = x;
  cursor->y = y;

  return 0;
}

int cursor_draw(cursor_t *cursor) {
  if (cursor == NULL)
    return 1;

  switch (cursor->state) {
    case POINTER:
      sprite_move(cursor->pointer, cursor->x, cursor->y);
      return sprite_draw(cursor->pointer);
    case HAND:
      sprite_move(cursor->hand, cursor->x, cursor->y);
      return sprite_draw(cursor->hand);
  }

  return 1;
}

void cursor_destroy(cursor_t *cursor) {
  if (cursor == NULL)
    return;

  sprite_destroy(cursor->pointer);
  sprite_destroy(cursor->hand);
}

int cursor_sprite_colides(cursor_t *cursor, sprite_t *sprite) {
  mouse_info_t *info = mouse_get_info();

  if (cursor == NULL || sprite == NULL)
    return 0;

  sprite_t *sel_sprite = cursor->state == HAND ? cursor->hand : cursor->pointer;

  sprite_move(sel_sprite, cursor->x, cursor->y);

  if (cursor->state == HAND)
    return 0;

  cursor->state = sprite_colides(sel_sprite, sprite) ? HAND : POINTER;

  return cursor->state == HAND && info != NULL && info->lb;
}

int cursor_box_colides(cursor_t *cursor, uint32_t x, uint32_t y, uint32_t x2, uint32_t y2) {
  mouse_info_t *info = mouse_get_info();

  if (cursor == NULL)
    return 0;

  sprite_t *sel_sprite = cursor->state == HAND ? cursor->hand : cursor->pointer;

  sprite_move(sel_sprite, cursor->x, cursor->y);

  if (cursor->state == HAND)
    return 0;

  uint8_t x_colides = cursor->x >= x && cursor->x <= x2;
  uint8_t y_colides = cursor->y >= y && cursor->y <= y2;
  cursor->state = x_colides && y_colides ? HAND : POINTER;

  return cursor->state == HAND && info != NULL && info->lb;
}
