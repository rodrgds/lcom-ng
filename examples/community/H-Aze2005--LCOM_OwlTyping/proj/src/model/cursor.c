#include <lcom/lcf.h>

#include "cursor.h"


Cursor *construct_cursor(int16_t x, int16_t y) {
  Cursor *cursor = (Cursor *) malloc(sizeof(Cursor));

  if (!cursor) {
    printf("%s: malloc() error\n", __func__);
    return NULL;
  }

  cursor->x = x;
  cursor->y = y;
  cursor->prev_cursor_x = x;
  cursor->prev_cursor_y = y;

  return cursor;
}

int move_cursor(Cursor *cursor, int16_t delta_x, int16_t delta_y) {
  if (!cursor) {
    printf("%s: cursor is NULL\n", __func__);
    return 1;
  }

  cursor->x += delta_x;
  if (cursor->x < 0) cursor->x = 0;
  if (cursor->x > 1024-27) cursor->x = 1024-27;
  cursor->y -= delta_y;
  if (cursor->y < 0) cursor->y = 0;
  if (cursor->y > 768-29) cursor->y = 768-29;

  return 0;
}

void destroy_cursor(Cursor *cursor) {
  if (cursor)
    free(cursor);
}

void draw_cursor(Cursor *cursor) {
    redraw_background(cursor->prev_cursor_x, cursor->prev_cursor_y, 27, 29);
    draw_image((xpm_map_t) cursor_xpm, cursor->x, cursor->y);
    cursor->prev_cursor_x = cursor->x;
    cursor->prev_cursor_y = cursor->y;
}

bool on_start_button(Cursor *cursor) {
    return (cursor->x > 262 && cursor->x < 762) && (cursor->y > 500 && cursor->y < 600);
}

bool on_top_right_button(Cursor *cursor) {
    return (cursor->x > 924 && cursor->x < 1004) && (cursor->y > 20 && cursor->y < 100);
}

bool on_continue_button(Cursor *cursor) {
    return (cursor->x > 312 && cursor->x < 712) && (cursor->y > 377 && cursor->y < 457);
}

bool on_pause_home(Cursor *cursor) {
    return (cursor->x > 393 && cursor->x < 473) && (cursor->y > 488 && cursor->y < 568);
}

bool on_pause_exit(Cursor *cursor) {
    return (cursor->x > 573 && cursor->x < 653) && (cursor->y > 488 && cursor->y < 568);
}

bool on_30s_button(Cursor *cursor) {
    return (cursor->x > 440 && cursor->x < 530) && (cursor->y > 417 && cursor->y < 457);
}

bool on_60s_button(Cursor *cursor) {
    return (cursor->x > 530 && cursor->x < 620) && (cursor->y > 417 && cursor->y < 457);
}

bool on_120s_button(Cursor *cursor) {
    return (cursor->x > 620 && cursor->x < 720) && (cursor->y > 417 && cursor->y < 457);
}

