#include <lcom/lcf.h>

#include "menu_view.h"

#include "../model/menu.h"

#include "button_view.h"
#include "cursor_view.h"

extern Button *play;
extern Button *quit;
extern Cursor *cursor_menu;

int draw_menu() {
  if (draw_button(play)) {
    printf("%s: draw_button(play) error\n", __func__);
    return 1;
  }

  if (draw_button(quit)) {
    printf("%s: draw_button(quit) error\n", __func__);
    return 1;
  }

  if (draw_cursor(cursor_menu)) {
    printf("%s: draw_cursor(cursor_menu) error\n", __func__);
    return 1;
  }

  return 0;
}
