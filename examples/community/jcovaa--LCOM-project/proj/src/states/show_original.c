#include "show_original.h"
#include "../drivers/video/graphics.h"
#include "../grid.h"
#include "../levels.h"
#include "user_paint.h"
#include "util_states.h"
#include "../ui_assets.h"

extern int current_level;

extern uint16_t x_resolution;
extern uint16_t y_resolution;
static int show_original_duration = 0;
static const int SHOW_ORIGINAL_MAX = 900; // 15 seconds at 60Hz
static const int SHOW_ORIGINAL_REVISIT_MAX = 180; // 3 seconds at 60Hz 
static bool is_revisit_mode = false;

void init_show_original() {
  show_original_duration = 0;
  is_revisit_mode = false;
}

void init_show_original_revisit() {
  show_original_duration = 0;
  is_revisit_mode = true;
}

void draw_show_original(GameState *state) {
  const char* title[] = {"YOU", "HAVE", "TO", "RECREATE", "THIS", "DRAWING"};
  const char* revisit_title[] = {"QUICK", "LOOK"};

  if (show_original_duration == 0 && !is_revisit_mode) {
    load_level(current_level);
    grid_init();
  }

  set_white_background();
  draw_original_drawing();
  draw_grid_outline(GRID_ROWS, GRID_COLS, CELL_SIZE, CELL_SIZE, GRID_ORIGIN_X, GRID_ORIGIN_Y, 0x000000);

  if (is_revisit_mode) {
    draw_centered_paragraph(revisit_title, sizeof(revisit_title) / sizeof(revisit_title[0]), 50, 10, 3);

    int remaining_seconds = (SHOW_ORIGINAL_REVISIT_MAX - show_original_duration) / 60; // frequency of the timer
    char seconds_str[3];
    sprintf(seconds_str, "%d", remaining_seconds);
    draw_text(seconds_str, x_resolution / 2 - 10, 100, 3);

    show_original_duration++;
    if (show_original_duration >= SHOW_ORIGINAL_REVISIT_MAX) {
      show_original_duration = 0;
      is_revisit_mode = false;
      *state = USER_PAINT;
    }
  }
  else {
    draw_centered_paragraph(title, sizeof(title) / sizeof(title[0]), 50, 10, 3);

    int remaining_seconds = (SHOW_ORIGINAL_MAX - show_original_duration) / 60; // frequency of the timer
    char seconds_str[3];
    sprintf(seconds_str, "%d", remaining_seconds);
    draw_text(seconds_str, x_resolution / 2 - 10, 100, 3);

    show_original_duration++;
    if (show_original_duration >= SHOW_ORIGINAL_MAX) {
      show_original_duration = 0;
      init_user_paint();
      *state = USER_PAINT;
    }
  }
}
