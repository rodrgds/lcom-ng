#include <lcom/lcf.h>
#include "model.h"

State current_state = HOME;
Cursor *cursor = NULL;
TimeSelector *timeSelector = NULL;

void set_state(State state) {
  current_state = state;
  draw_screen();
}

void start_game() {
  cursor = construct_cursor(290, 410);
  timeSelector = construct_TimeSelector(288, 407);
}
