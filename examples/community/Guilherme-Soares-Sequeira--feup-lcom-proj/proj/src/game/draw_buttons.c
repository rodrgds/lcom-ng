#include <lcom/lcf.h>

#include "../devices/video/graphics.h"
#include "draw_buttons.h"

/* button xpms */

static xpm_image_t button_plus;         /**< @brief xpm image of the 'plus' button         */
static xpm_image_t button_minus;        /**< @brief xpm image of the 'minus' button        */
static xpm_image_t button_eraser;       /**< @brief xpm image of the 'eraser' button       */
static xpm_image_t button_bucket;       /**< @brief xpm image of the 'bucket' button       */
static xpm_image_t button_text;         /**< @brief xpm image of the 'text' button         */
static xpm_image_t button_clear_canvas; /**< @brief xpm image of the 'clear canvas' button */

xpm_image_t(get_button_plus)() { return button_plus; }

void(load_button_xpms)() {
  button_plus = vg_load_xpm(xpm_button_plus);
  button_minus = vg_load_xpm(xpm_button_minus);
  button_eraser = vg_load_xpm(xpm_button_eraser);
  button_bucket = vg_load_xpm(xpm_button_bucket);
  button_text = vg_load_xpm(xpm_button_text);
  button_clear_canvas = vg_load_xpm(xpm_button_clear_canvas);
}

void(draw_button_frame)(pixel_buffer const *const buf, position topleft_pos) {
  buf_draw_hline(buf, topleft_pos, BUTTON_FRAME_WIDTH, BUTTON_FRAME_COLOR);
  buf_draw_hline(buf, (position){topleft_pos.x, topleft_pos.y + 1}, BUTTON_FRAME_WIDTH, BUTTON_FRAME_COLOR);
  buf_draw_hline(buf, (position){topleft_pos.x, topleft_pos.y + BUTTON_FRAME_HEIGHT - 2}, BUTTON_FRAME_WIDTH, BUTTON_FRAME_COLOR);
  buf_draw_hline(buf, (position){topleft_pos.x, topleft_pos.y + BUTTON_FRAME_HEIGHT - 1}, BUTTON_FRAME_WIDTH, BUTTON_FRAME_COLOR);

  buf_draw_vline(buf, (position){topleft_pos.x, topleft_pos.y + 2}, BUTTON_FRAME_HEIGHT - 4, BUTTON_FRAME_COLOR);
  buf_draw_vline(buf, (position){topleft_pos.x + 1, topleft_pos.y + 2}, BUTTON_FRAME_HEIGHT - 4, BUTTON_FRAME_COLOR);
  buf_draw_vline(buf, (position){topleft_pos.x + BUTTON_FRAME_WIDTH - 2, topleft_pos.y + 2}, BUTTON_FRAME_HEIGHT - 4, BUTTON_FRAME_COLOR);
  buf_draw_vline(buf, (position){topleft_pos.x + BUTTON_FRAME_WIDTH - 1, topleft_pos.y + 2}, BUTTON_FRAME_HEIGHT - 4, BUTTON_FRAME_COLOR);
}

int(draw_color_button)(pixel_buffer const *const buf, position topleft_pos, uint8_t color) {
  draw_button_frame(buf, topleft_pos);

  if (buf_draw_rectangle(buf, (position){topleft_pos.x + 2, topleft_pos.y + 2}, BUTTON_FRAME_WIDTH - 4, BUTTON_FRAME_HEIGHT - 4, color) != OK) {
    fprintf(stderr, "There was an error drawing a rectangle at %s!\n", __func__);
    return EXIT_FAILURE;
  }

  return OK;
}

void(draw_plus_button)(pixel_buffer const *const buf, position topleft_pos) {
  buf_draw_xpm(buf, button_plus, topleft_pos);
}

void(draw_minus_button)(pixel_buffer const *const buf, position topleft_pos) {
  buf_draw_xpm(buf, button_minus, topleft_pos);
}

int(draw_sel_line_button)(pixel_buffer const *const buf, position topleft_pos) {
  draw_button_frame(buf, topleft_pos);

  if (buf_draw_rectangle(buf, (position){topleft_pos.x + 18, topleft_pos.y + 6}, 6, BUTTON_FRAME_HEIGHT - 14, COLOR_BLACK) != OK) {
    fprintf(stderr, "There was an error drawing a rectangle at %s!\n", __func__);
    return EXIT_FAILURE;
  }

  return OK;
}

int(draw_sel_square_button)(pixel_buffer const *const buf, position topleft_pos) {
  draw_button_frame(buf, topleft_pos);

  if (buf_draw_rectangle(buf, (position){topleft_pos.x + 12, topleft_pos.y + 12}, BUTTON_FRAME_WIDTH - 24, BUTTON_FRAME_HEIGHT - 24, COLOR_BLACK) != OK) {
    fprintf(stderr, "There was an error drawing a rectangle at %s!\n", __func__);
    return EXIT_FAILURE;
  }

  return OK;
}

int(draw_sel_circle_button)(pixel_buffer const *const buf, position topleft_pos) {
  draw_button_frame(buf, topleft_pos);

  if (buf_draw_circle(buf, (position){topleft_pos.x + 20, topleft_pos.y + 20}, 9, COLOR_BLACK) != OK) {
    fprintf(stderr, "There was an error drawing a rectangle at %s!\n", __func__);
    return EXIT_FAILURE;
  }

  return OK;
}

void(draw_sel_bucket_button)(pixel_buffer const *const buf, position topleft_pos) {
  buf_draw_xpm(buf, button_bucket, topleft_pos);
}

void(draw_sel_text_button)(pixel_buffer const *const buf, position topleft_pos) {
  buf_draw_xpm(buf, button_text, topleft_pos);
}

void(draw_sel_eraser_button)(pixel_buffer const *const buf, position topleft_pos) {
  draw_button_frame(buf, topleft_pos);
  buf_draw_xpm(buf, button_eraser, topleft_pos);
}

void(draw_clear_canvas_button)(pixel_buffer const *const buf, position topleft_pos) {
  buf_draw_xpm(buf, button_clear_canvas, topleft_pos);
}

int(buttons_draw)() {
  const pixel_buffer *const buf = get_back_buffer();

  const int color_button_x = CANVAS_RIGHT_VISIBLE_LIMIT + (CANVAS_MARGIN - BUTTON_FRAME_WIDTH) / 2;
  const int other_buttons_x = (CANVAS_MARGIN - BUTTON_FRAME_WIDTH) / 2;

  if (draw_color_button(buf, (position){color_button_x, 70}, COLOR_BLACK) != OK) {
    fprintf(stderr, "There was an error drawing a color button at %s!\n", __func__);
    return EXIT_FAILURE;
  }

  if (draw_color_button(buf, (position){color_button_x, 120}, COLOR_BLUE) != OK) {
    fprintf(stderr, "There was an error drawing a color button at %s!\n", __func__);
    return EXIT_FAILURE;
  }

  if (draw_color_button(buf, (position){color_button_x, 170}, COLOR_GREEN) != OK) {
    fprintf(stderr, "There was an error drawing a color button at %s!\n", __func__);
    return EXIT_FAILURE;
  }

  if (draw_color_button(buf, (position){color_button_x, 220}, COLOR_RED) != OK) {
    fprintf(stderr, "There was an error drawing a color button at %s!\n", __func__);
    return EXIT_FAILURE;
  }

  if (draw_color_button(buf, (position){color_button_x, 270}, COLOR_BROWN) != OK) {
    fprintf(stderr, "There was an error drawing a color button at %s!\n", __func__);
    return EXIT_FAILURE;
  }

  if (draw_color_button(buf, (position){color_button_x, 320}, COLOR_YELLOW) != OK) {
    fprintf(stderr, "There was an error drawing a color button at %s!\n", __func__);
    return EXIT_FAILURE;
  }

  if (draw_color_button(buf, (position){color_button_x, 370}, COLOR_AQUA) != OK) {
    fprintf(stderr, "There was an error drawing a color button at %s!\n", __func__);
    return EXIT_FAILURE;
  }

  if (draw_color_button(buf, (position){color_button_x, 420}, COLOR_MAGENTA) != OK) {
    fprintf(stderr, "There was an error drawing a color button at %s!\n", __func__);
    return EXIT_FAILURE;
  }

  if (draw_color_button(buf, (position){color_button_x, 470}, COLOR_PURPLE) != OK) {
    fprintf(stderr, "There was an error drawing a color button at %s!\n", __func__);
    return EXIT_FAILURE;
  }

  if (draw_color_button(buf, (position){color_button_x, 520}, COLOR_ORANGE) != OK) {
    fprintf(stderr, "There was an error drawing a color button at %s!\n", __func__);
    return EXIT_FAILURE;
  }

  if (draw_color_button(buf, (position){color_button_x, 570}, COLOR_DARK_GREY) != OK) {
    fprintf(stderr, "There was an error drawing a color button at %s!\n", __func__);
    return EXIT_FAILURE;
  }

  if (draw_color_button(buf, (position){color_button_x, 620}, COLOR_WHITE) != OK) {
    fprintf(stderr, "There was an error drawing a color button at %s!\n", __func__);
    return EXIT_FAILURE;
  }

  draw_plus_button(buf, (position){other_buttons_x, 70});
  draw_minus_button(buf, (position){other_buttons_x, 120});

  if (draw_sel_square_button(buf, (position){other_buttons_x, 170}) != OK) {
    fprintf(stderr, "There was an error drawing a square button at %s!\n", __func__);
    return EXIT_FAILURE;
  }

  if (draw_sel_circle_button(buf, (position){other_buttons_x, 220}) != OK) {
    fprintf(stderr, "There was an error drawing a circle button at %s!\n", __func__);
    return EXIT_FAILURE;
  }

  if (draw_sel_line_button(buf, (position){other_buttons_x, 270}) != OK) {
    fprintf(stderr, "There was an error drawing a line button at %s!\n", __func__);
    return EXIT_FAILURE;
  }

  draw_sel_eraser_button(buf, (position){other_buttons_x, 320});

  draw_sel_bucket_button(buf, (position){10, 370});

  draw_sel_text_button(buf, (position){10, 420});

  draw_clear_canvas_button(buf, (position){other_buttons_x, 670});

  return OK;
}
