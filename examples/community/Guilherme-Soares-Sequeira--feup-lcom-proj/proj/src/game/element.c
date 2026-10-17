#include <lcom/lcf.h>

#include "element.h"

static interactive_element *drawing_ies; /**< @brief the container of the interactive elements */

static uint8_t number_of_drawing_ies; /**< @brief the number of interactive elements present on screen */

interactive_element(create_color_button)(position pos, uint8_t color) {
  return (interactive_element){
    pos,
    BUTTON_FRAME_WIDTH,
    BUTTON_FRAME_HEIGHT,
    color,
    COLOR_BUTTON_PRIORITY,
    cursor_set_color};
}

interactive_element(create_style_button)(position pos, enum cursor_style style) {
  return (interactive_element){
    pos,
    BUTTON_FRAME_WIDTH,
    BUTTON_FRAME_HEIGHT,
    style,
    STYLE_BUTTON_PRIORITY,
    cursor_set_style};
}

void(load_ie_drawing)() {

  /* create color buttons */

  interactive_element black_color_button = create_color_button(
    (position){974, 70}, COLOR_BLACK);

  interactive_element blue_color_button = create_color_button(
    (position){974, 120}, COLOR_BLUE);

  interactive_element green_color_button = create_color_button(
    (position){974, 170}, COLOR_GREEN);

  interactive_element red_color_button = create_color_button(
    (position){974, 220}, COLOR_RED);

  interactive_element brown_color_button = create_color_button(
    (position){974, 270}, COLOR_BROWN);

  interactive_element yellow_color_button = create_color_button(
    (position){974, 320}, COLOR_YELLOW);

  interactive_element aqua_color_button = create_color_button(
    (position){974, 370}, COLOR_AQUA);

  interactive_element magenta_color_button = create_color_button(
    (position){974, 420}, COLOR_MAGENTA);

  interactive_element purple_color_button = create_color_button(
    (position){974, 470}, COLOR_PURPLE);

  interactive_element orange_color_button = create_color_button(
    (position){974, 520}, COLOR_ORANGE);

  interactive_element dark_grey_color_button = create_color_button(
    (position){974, 570}, COLOR_DARK_GREY);

  interactive_element white_color_button = create_color_button(
    (position){974, 620}, COLOR_WHITE);

  /* create cursor style buttons */

  interactive_element square_button = create_style_button(
    (position){10, 170}, CURSOR_DSTATE_SQUARE);

  interactive_element circle_button = create_style_button(
    (position){10, 220}, CURSOR_DSTATE_CIRCLE);

  interactive_element sel_line_button = create_style_button(
    (position){10, 270}, CURSOR_DSTATE_LINE);

  interactive_element eraser_button = create_style_button(
    (position){10, 320}, CURSOR_DSTATE_ERASER);

  interactive_element sel_bucket_button = create_style_button(
    (position){10, 370}, CURSOR_DSTATE_BUCKET);

  interactive_element sel_draw_text_button = create_style_button(
    (position){10, 420}, CURSOR_DSTATE_TEXT);

  /* other buttons */

  interactive_element plus_button = {
    (position){10, 70},
    40,
    40,
    COLOR_TRANSPARENT,
    3,
    cursor_increase_thickness};

  interactive_element minus_button = {
    (position){10, 120},
    40,
    40,
    COLOR_TRANSPARENT,
    3,
    cursor_decrease_thickness};

  interactive_element clear_canvas_button = {
    (position){10, 670},
    40,
    40,
    COLOR_WHITE,
    3,
    canvas_fill};

  /* canvas */

  interactive_element canvas = {
    (position){CANVAS_LEFT_VISIBLE_LIMIT, CANVAS_TOP_VISIBLE_LIMIT},
    CANVAS_WIDTH,
    CANVAS_HEIGHT,
    COLOR_TRANSPARENT,
    1,
    canvas_mouse_handler};

  number_of_drawing_ies = 23;

  drawing_ies = (interactive_element *) malloc(number_of_drawing_ies * sizeof(interactive_element));

  memcpy(drawing_ies, (interactive_element[]){canvas, black_color_button, blue_color_button, green_color_button, red_color_button, brown_color_button, yellow_color_button, aqua_color_button, magenta_color_button, purple_color_button, orange_color_button, dark_grey_color_button, plus_button, minus_button, square_button, sel_bucket_button, circle_button, eraser_button, clear_canvas_button, sel_line_button, sel_bucket_button, sel_draw_text_button, white_color_button}, number_of_drawing_ies * sizeof(interactive_element));
}

bool(is_hovered)(interactive_element elem) {
  position mouse_position = cursor_get_pos();

  return (mouse_position.x >= elem.top_pos.x &&
          mouse_position.x < elem.top_pos.x + elem.h_len &&
          mouse_position.y >= elem.top_pos.y &&
          mouse_position.y < elem.top_pos.y + elem.v_len);
}

uint8_t(get_number_of_drawing_ies)() {
  return number_of_drawing_ies;
}

interactive_element *(get_drawing_ies) () {
  return drawing_ies;
}
