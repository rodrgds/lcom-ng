#include <lcom/lcf.h>

#include "levels_view.h"

#include "../model/levels.h"

#include "button_view.h"
#include "cursor_view.h"

extern Button *easy;
extern Button *medium;
extern Button *hard;
extern Cursor *cursor_levels;

int draw_levels() {
  if (draw_button(easy)) {
    printf("%s: draw_button(easy) error\n", __func__);
    return 1;
  }

  if (draw_button(medium)) {
    printf("%s: draw_button(medium) error\n", __func__);
    return 1;
  }

  if (draw_button(hard)) {
    printf("%s: draw_button(hard) error\n", __func__);
    return 1;
  }

  if (draw_cursor(cursor_levels)) {
    printf("%s: draw_cursor(cursor_levels) error\n", __func__);
    return 1;
  }

  return 0;
}
