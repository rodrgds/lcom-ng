#include "timer_event_handler.h"


void handle_timer_event(int game_state) {
  switch (state) {
    case MAIN_MENU:
      handle_menu();
      break;
      
    case PLAYING:
      handle_game();
      break;

    case INSTRUCTIONS:
      handle_instructions();
      break;
      
    case EXIT:
      break;
  }
}

void handle_menu() {
  //draw_title_screen();
}

void handle_game() {
  //draw_board();
}

void handle_instructions() {
  // draw_instructions();
}
