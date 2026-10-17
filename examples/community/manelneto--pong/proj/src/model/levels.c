#include <lcom/lcf.h>

#include "levels.h"

#include "button.h"
#include "cursor.h"

#include "../controller/keyboard.h"
#include "../controller/mouse.h"

#include "../xpm/button_easy.xpm"
#include "../xpm/button_easy_selected.xpm"
#include "../xpm/button_hard.xpm"
#include "../xpm/button_hard_selected.xpm"
#include "../xpm/button_medium.xpm"
#include "../xpm/button_medium_selected.xpm"

extern scancode code;
extern struct packet mouse_packet;

Button *easy = NULL;
Button *medium = NULL;
Button *hard = NULL;
Cursor *cursor_levels = NULL;

static uint16_t x_max;
static uint16_t y_max;

static int16_t x;
static int16_t y;

int start_levels(uint16_t xResolution, uint16_t yResolution) {
  easy = construct_button(xResolution / 3, yResolution / 7, xResolution / 3, yResolution / 7, button_easy_xpm, button_easy_selected_xpm);
  if (!easy) {
    printf("%s: construct_button(%d, %d, %d, %d, button_easy_xpm, button_easy_selected_xpm) error\n", __func__, xResolution / 3, yResolution / 7, xResolution / 3, yResolution / 7);
    return 1;
  }

  medium = construct_button(xResolution / 3, 3 * yResolution / 7, xResolution / 3, yResolution / 7, button_medium_xpm, button_medium_selected_xpm);
  if (!medium) {
    printf("%s: construct_button(%d, %d, %d, %d, button_medium_xpm, button_medium_selected_xpm) error\n", __func__, xResolution / 3, 3 * yResolution / 7, xResolution / 3, yResolution / 7);
    return 1;
  }

  hard = construct_button(xResolution / 3, 5 * yResolution / 7, xResolution / 3, yResolution / 7, button_hard_xpm, button_hard_selected_xpm);
  if (!hard) {
    printf("%s: construct_button(%d, %d, %d, %d, button_hard_xpm, button_hard_selected_xpm) error\n", __func__, xResolution / 3, 5 * yResolution / 7, xResolution / 3, yResolution / 7);
    return 1;
  }

  cursor_levels = construct_cursor(xResolution / 2, yResolution / 2);
  if (!cursor_levels) {
    printf("%s: construct_cursor(%d, %d) error\n", __func__, xResolution / 2, yResolution / 2);
    return 1;
  }

  x_max = xResolution;
  y_max = yResolution;

  x = cursor_levels->x;
  y = cursor_levels->y;

  return 0;
}

void keyboard_levels_handler() {
  if (code.size == 1 && (code.bytes[0] == KBD_1_MAKECODE || code.bytes[0] == KBD_E_MAKECODE))
    easy->selected = true;
  else if (code.size == 1 && (code.bytes[0] == KBD_2_MAKECODE || code.bytes[0] == KBD_M_MAKECODE))
    medium->selected = true;
  else if (code.size == 1 && (code.bytes[0] == KBD_3_MAKECODE || code.bytes[0] == KBD_H_MAKECODE))
    hard->selected = true;
}

void mouse_levels_handler() {
  x += mouse_packet.delta_x;
  if (x < 0)                                          // colisão com o limite esquerdo do ecrã
    x = 0;
  else if (x + cursor_levels->sprite->width >= x_max) // colisão com o limite direito do ecrã
    x = x_max - cursor_levels->sprite->width - 1;

  y -= mouse_packet.delta_y;
  if (y < 0)                                           // colisão com o limite superior do ecrã
    y = 0;
  else if (y + cursor_levels->sprite->height >= y_max) // colisão com o limite inferior do ecrã
    y = y_max - cursor_levels->sprite->height - 1;

  move_cursor(cursor_levels, x, y);

  check_button(easy, cursor_levels->x, cursor_levels->y);
  check_button(medium, cursor_levels->x, cursor_levels->y);
  check_button(hard, cursor_levels->x, cursor_levels->y);
}

bool check_easy() {
  return easy->selected;
}

bool check_medium() {
  return medium->selected;
}

bool check_hard() {
  return hard->selected;
}

void end_levels() {
  destroy_button(easy);
  destroy_button(medium);
  destroy_button(hard);
  destroy_cursor(cursor_levels);
}
