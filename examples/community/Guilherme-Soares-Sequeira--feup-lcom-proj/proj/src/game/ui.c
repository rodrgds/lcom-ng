#include "ui.h"
#include "text.h"

void(draw_menu)() {
  pixel_buffer const *const back_buf = get_back_buffer();
  buf_draw_rectangle(back_buf, (position){0, 0}, H_RES, CANVAS_TOP_VISIBLE_LIMIT, COLOR_CYAN);
  buf_draw_rectangle(back_buf, (position){0, CANVAS_BOTTOM_VISIBLE_LIMIT - 60}, H_RES, 60, COLOR_CYAN);

  buf_draw_rectangle(back_buf, (position){0, CANVAS_TOP_VISIBLE_LIMIT}, CANVAS_LEFT_VISIBLE_LIMIT, CANVAS_BOTTOM_VISIBLE_LIMIT - 120, COLOR_CYAN);
  buf_draw_rectangle(back_buf, (position){CANVAS_RIGHT_VISIBLE_LIMIT, CANVAS_TOP_VISIBLE_LIMIT}, H_RES - CANVAS_RIGHT_VISIBLE_LIMIT, CANVAS_BOTTOM_VISIBLE_LIMIT - 120, COLOR_CYAN);

  buttons_draw();
}
