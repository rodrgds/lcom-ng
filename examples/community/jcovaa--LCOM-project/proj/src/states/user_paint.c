#include "user_paint.h"
#include "../drivers/video/graphics.h"
#include "../drivers/kbc/keyboard.h"
#include "../drivers/kbc/mouse.h"
#include "../grid.h"
#include "../ui_assets.h"
#include "show_original.h"
#include "util_states.h"
#include "results.h"

extern uint8_t scancode;
extern uint16_t x_resolution;
extern uint16_t y_resolution;

extern uint16_t mouse_x;
extern uint16_t mouse_y;
extern struct packet mouse_packet;

extern int current_level;

static int user_paint_duration = 0;
static const int USER_PAINT_MAX_DURATION = 1200; //  20 seconds at 60Hz
static bool time_up = false;
static bool revisit_used = false;

void init_user_paint() {
  user_paint_duration = 0;
  time_up = false;
  revisit_used = false;

  grid_clear();
}

void draw_user_paint(GameState *state) {
  const char* title[] = {"YOUR", "PAINTING"};

  set_white_background();

  grid_draw();

  draw_centered_paragraph(title, sizeof(title) / sizeof(title[0]), 50, 10, 3);

  int remaining_seconds = (USER_PAINT_MAX_DURATION - user_paint_duration) / 60; // frequency of the timer

  char seconds_str[3];
  sprintf(seconds_str, "%d", remaining_seconds);

  draw_text(seconds_str, x_resolution / 2 - 10, 100, 3);

  if (!revisit_used) {
    uint16_t revisit_text_x = x_resolution - 180;

    draw_text("CLICK", revisit_text_x, 210, 3);
    draw_text("ENTER", revisit_text_x, 240, 3);
    draw_text("TO", revisit_text_x, 270, 3);
    draw_text("SEE", revisit_text_x, 300, 3);
    draw_text("ORIGINAL", revisit_text_x, 330, 3);
    draw_text("AGAIN", revisit_text_x, 360, 3);
  }

  handle_mouse_painting();

  user_paint_duration++;
  if (user_paint_duration >= USER_PAINT_MAX_DURATION) {
    time_up = true;

    bool success = compare_grid();

    init_results(success, current_level);
    *state = RESULTS;
  }
}

void handle_mouse_painting() {
  static bool left_pressed = false;
  static bool right_pressed = false;
  static uint8_t last_painted_row = 255;   // initializing to an invalid value
  static uint8_t last_painted_col = 255;   // initializing to an invalid value

  uint8_t row, col;

  bool valid_cell = (get_cell(mouse_x, mouse_y, &row, &col) == 0);

  if (mouse_packet.lb) {
    left_pressed = true;
    if (valid_cell && (row != last_painted_row || col != last_painted_col)) {
      grid_paint_cell(row, col, 0x000000);
      last_painted_row = row;
      last_painted_col = col;
    }
  }
  else {
    left_pressed = false;
  }

  if (mouse_packet.rb) {
    right_pressed = true;
    if (valid_cell && (row != last_painted_row || col != last_painted_col)) {
      grid_paint_cell(row, col, 0xFFFFFF);
      last_painted_row = row;
      last_painted_col = col;
    }
  }
  else {
    right_pressed = false;
  }

  // If neither mouse button is pressed, reset the last painted cell to an invalid value
  if (!left_pressed && !right_pressed) {   
    last_painted_row = 255;
    last_painted_col = 255;
  }
}

void handle_user_paint_input(GameState *state) {
  if (scancode != 0) {
    if (scancode == ESC_KEY) {
      *state = EXIT_GAME;
    }
    else if (scancode == ENTER_KEY && !revisit_used) {
      revisit_used = true;
      init_show_original_revisit();
      *state = SHOW_ORIGINAL;
    }
    scancode = 0;
  }
}
