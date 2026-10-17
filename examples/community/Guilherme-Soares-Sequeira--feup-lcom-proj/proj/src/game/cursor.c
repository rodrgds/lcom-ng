#include <lcom/lcf.h>
#include <lcom/video_gr.h>

#include "../xpm/bucket_cursor.xpm"
#include "../xpm/cursor.xpm"
#include "../xpm/eraser_cursor.xpm"
#include "../xpm/line_cursor.xpm"
#include "../xpm/pencil.xpm"
#include "../xpm/text_cursor.xpm"
#include "cursor.h"

static xpm_image_t cursor_default_xpm; /**< @brief default cursor xpm */
static xpm_image_t cursor_bucket_xpm;  /**< @brief bucket cursor xpm  */
static xpm_image_t cursor_eraser_xpm;  /**< @brief eraser cursor xpm  */
static xpm_image_t cursor_pencil_xpm;  /**< @brief pencil cursor xpm  */
static xpm_image_t cursor_text_xpm;    /**< @brief text cursor xpm    */
static xpm_image_t cursor_line_xpm;    /**< @brief line cursor xpm    */

static position cursor_pos = {0, 0}; /**< @brief cursor_position of the cursor */

static cursor_state_style_t cursor_style; /**< @brief style of the cursor */

static uint8_t cursor_color;        /**< @brief color/previous of the cursor */
static uint8_t cursor_thickness;    /**< @brief thickness of the cursor */
static uint8_t cursor_line_counter; /**< @brief line counter */

static position line_initial_position; /**< @brief initial position of the line to be drawn */
static position text_initial_position; /**< @brief initial position of the text to be drawn */

static bool cursor_lb; /**< @brief indicates whether the mouse left button was already pressed or not */

static bool typing; /**< @brief indicates whether the user is typing or not */

void(cursor_load)() {
  cursor_line_xpm = vg_load_xpm(xpm_cursor_line);
  cursor_default_xpm = vg_load_xpm(xpm_cursor_arrow);
  cursor_bucket_xpm = vg_load_xpm(xpm_cursor_bucket);
  cursor_eraser_xpm = vg_load_xpm(xpm_cursor_eraser);
  cursor_pencil_xpm = vg_load_xpm(xpm_cursor_pencil);
  cursor_text_xpm = vg_load_xpm(xpm_cursor_text);

  cursor_pos.x = H_RES / 2;
  cursor_pos.y = V_RES / 2;

  cursor_line_counter = 0;
  cursor_color = COLOR_BLACK;
  cursor_thickness = (MIN_THICKNESS + MAX_THICKNESS) / 2;
  cursor_style = CURSOR_DSTATE_CIRCLE;

  cursor_lb = false;
  typing = false;
}

xpm_image_t(cursor_get_xpm)() {
  /* in the future this needs to be changed */
  switch (cursor_style) {
    case CURSOR_DSTATE_CIRCLE: return cursor_pencil_xpm;
    case CURSOR_DSTATE_SQUARE: return cursor_pencil_xpm;
    case CURSOR_DSTATE_TEXT: return cursor_text_xpm;
    case CURSOR_DSTATE_LINE: return cursor_line_xpm;
    case CURSOR_DSTATE_ERASER: return cursor_eraser_xpm;
    case CURSOR_DSTATE_BUCKET: return cursor_bucket_xpm;
    default: return cursor_default_xpm;
  }
}

void(cursor_move)(int16_t delta_x, int16_t delta_y) {
  cursor_pos.x = MIN(MAX(0, cursor_pos.x + delta_x), H_RES - cursor_get_xpm().width);
  cursor_pos.y = MIN(MAX(0, cursor_pos.y - delta_y), V_RES - cursor_get_xpm().height);
}

void(cursor_draw)() {
  position pos = cursor_pos;
  if (!is_hovered(get_drawing_ies()[0])) {
    if (vg_draw_xpm(cursor_default_xpm, pos.x, pos.y) != OK) {
      fprintf(stderr, "There was an error drawing an xpm at %s!\n", __func__);
      exit(EXIT_FAILURE);
    }
    return;
  }
  xpm_image_t xpm = cursor_get_xpm();
  cursor_state_style_t style = cursor_get_style();
  switch (style) {
    case CURSOR_DSTATE_ERASER:
      pos.x -= 15;
    case CURSOR_DSTATE_BUCKET:
    case CURSOR_DSTATE_TEXT:
      pos.y -= 30;
      break;
    case CURSOR_DSTATE_CIRCLE:
    case CURSOR_DSTATE_SQUARE:
      pos.y -= 30;
      break;
    case CURSOR_DSTATE_LINE:
      pos.x -= 11;
      pos.y -= 11;
    default:
      break;
  }

  if (vg_draw_xpm(xpm, pos.x, pos.y) != OK) {
    fprintf(stderr, "There was an error drawing an xpm at %s!\n", __func__);
    exit(EXIT_FAILURE);
  }
}


void(cursor_set_initial_line_position)(position pos) {
  line_initial_position = pos;
}

position(cursor_get_initial_line_position)() {
  return line_initial_position;
}

void(cursor_set_style)(uint8_t style) {
  if ((cursor_state_style_t) style == CURSOR_DSTATE_LINE) {
    cursor_line_counter = 0;
  }

  cursor_style = (cursor_state_style_t) style;
}

void(cursor_increase_line_counter)() {
  cursor_line_counter = (cursor_line_counter + 1) % 2;
}

uint8_t(cursor_get_line_counter)() {
  return cursor_line_counter;
}

cursor_state_style_t(cursor_get_style)() {
  return cursor_style;
}

void(cursor_set_thickness)(uint8_t thickness) {
  cursor_thickness = thickness;
}

uint8_t(cursor_get_thickness)() {
  return cursor_thickness;
}

position(cursor_get_pos)() {
  return cursor_pos;
}

bool(cursor_lb_was_pressed)() {
  return cursor_lb;
}

void(cursor_set_lb)(bool lb) {
  cursor_lb = lb;
}

uint8_t(cursor_get_color)() {
  return cursor_color;
}

void(cursor_set_color)(uint8_t color) {
  cursor_color = color;
}

void(cursor_increase_thickness)(uint8_t _) {
  cursor_thickness = cursor_thickness + 1 > MAX_THICKNESS ? MAX_THICKNESS : cursor_thickness + 1;
}

void(cursor_decrease_thickness)(uint8_t _) {
  cursor_thickness = cursor_thickness - 1 < MIN_THICKNESS ? MIN_THICKNESS : cursor_thickness - 1;
}

position(get_text_initial_position)() {
  return text_initial_position;
}

void(set_text_initial_position)(position pos) {
  text_initial_position = pos;
}

bool(get_is_typing)() {
  return typing;
}

void(set_is_typing)(bool new_is_typing) {
  typing = new_is_typing;
}
