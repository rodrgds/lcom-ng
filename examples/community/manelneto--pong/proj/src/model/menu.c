#include <lcom/lcf.h>

#include "menu.h"

#include "button.h"
#include "cursor.h"

#include "../controller/keyboard.h"
#include "../controller/mouse.h"

#include "../xpm/button_play.xpm"
#include "../xpm/button_play_selected.xpm"
#include "../xpm/button_quit.xpm"
#include "../xpm/button_quit_selected.xpm"

extern scancode code;
extern struct packet mouse_packet;

Button *play = NULL;
Button *quit = NULL;
Cursor *cursor_menu = NULL;

static uint16_t x_max;
static uint16_t y_max;

static int16_t x;
static int16_t y;

int start_menu(uint16_t xResolution, uint16_t yResolution) {
  play = construct_button(xResolution / 3, yResolution / 5, xResolution / 3, yResolution / 5, button_play_xpm, button_play_selected_xpm);
  if (!play) {
    printf("%s: construct_button(%d, %d, %d, %d, button_play_xpm, button_play_selected_xpm) error\n", __func__, xResolution / 3, yResolution / 5, xResolution / 3, yResolution / 5);
    return 1;
  }

  quit = construct_button(xResolution / 3, 3 * yResolution / 5, xResolution / 3, yResolution / 5, button_quit_xpm, button_quit_selected_xpm);
  if (!quit) {
    printf("%s: construct_button(%d, %d, %d, %d, button_quit_xpm, button_quit_selected_xpm) error\n", __func__, xResolution / 3, 3 * yResolution / 5, xResolution / 3, yResolution / 5);
    return 1;
  }

  cursor_menu = construct_cursor(xResolution / 2, yResolution / 2);
  if (!cursor_menu) {
    printf("%s: construct_cursor(%d, %d) error\n", __func__, xResolution / 2, yResolution / 2);
    return 1;
  }

  x_max = xResolution;
  y_max = yResolution;

  x = cursor_menu->x;
  y = cursor_menu->y;

  return 0;
}

void keyboard_menu_handler() {
  if (code.size == 1 && (code.bytes[0] == KBD_1_MAKECODE || code.bytes[0] == KBD_P_MAKECODE))
    play->selected = true;
  else if (code.size == 1 && (code.bytes[0] == KBD_2_MAKECODE || code.bytes[0] == KBD_Q_MAKECODE))
    quit->selected = true;
}

void mouse_menu_handler() {
  x += mouse_packet.delta_x;
  if (x < 0)                                        // colisão com o limite esquerdo do ecrã
    x = 0;
  else if (x + cursor_menu->sprite->width >= x_max) // colisão com o limite direito do ecrã
    x = x_max - cursor_menu->sprite->width - 1;

  y -= mouse_packet.delta_y;
  if (y < 0)                                         // colisão com o limite superior do ecrã
    y = 0;
  else if (y + cursor_menu->sprite->height >= y_max) // colisão com o limite inferior do ecrã
    y = y_max - cursor_menu->sprite->height - 1;

  move_cursor(cursor_menu, x, y);

  check_button(play, cursor_menu->x, cursor_menu->y);
  check_button(quit, cursor_menu->x, cursor_menu->y);
}

bool check_play() {
  return play->selected;
}

bool check_quit() {
  return quit->selected;
}

void end_menu() {
  destroy_button(play);
  destroy_button(quit);
  destroy_cursor(cursor_menu);
}
