#include "wait_menu.h"

void draw_wait_menu(bool isDay) {
  if (isDay) {
    draw_background(dayDesert->map);
  }
  else {
    draw_background(nightDesert->map);
  }
  draw_sprite(waitMessage, MAX_X / 2, MAX_Y / 2);
  vg_page_flipping();
}
