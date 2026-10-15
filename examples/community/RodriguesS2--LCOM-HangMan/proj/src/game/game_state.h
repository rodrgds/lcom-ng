#ifndef GAME_STATE_H
#define GAME_STATE_H

#include <lcom/lcf.h>

typedef enum {
  MENU,
  CATEGORIES,
  PLAY,
  INSTRUCTIONS,
  EXIT
} StateOptions;

extern uint32_t timer_counter;
extern StateOptions state;
extern int selected_option;
extern int remaining_time;
extern int current_stage;
extern bool redraw_needed;

void handle_game_state();

void reset_game_state();

bool check_win();

#endif

