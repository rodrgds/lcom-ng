#include "menu/view/menu.h"

#include "graphics/graphics.h"

#include "devices/vbe.h"
#include "devices/mouse.h"
#include "menu/layout.h"

void (menu_draw)() {
  menu *m = menu_get();

  menu_draw_title();

  for (size_t i = 0; i < m->n; i++) {
    menu_option *opt = m->options + i;
    menu_draw_option(opt);
  }
}

void (menu_draw_title)() {
  graphics_draw_text(MENU_TITLE_X, MENU_TITLE_Y, MENU_TITLE);
}

void (menu_draw_option)(menu_option *opt) {
  uint16_t color = mouse_is_in_position(opt->x, opt->y, opt->x + opt->width, opt->y + opt->height) == 0
    ? MENU_OPTION_COLOR_NORMAL : MENU_OPTION_COLOR_HOVER; 

  uint16_t text_width = strlen(opt->description) * GRAPHICS_ALFA_WIDTH;

  vbe_draw_rectangle(opt->x, opt->y, opt->width, opt->height, color);
  graphics_draw_text(opt->x + (opt->width - text_width) / 2, opt->y + (opt->height - GRAPHICS_ALFA_HEIGHT) / 2, opt->description);
}
