#include "mouse_event_handler.h"

void handle_mouse_event(struct packet pp) {
  switch(state) {
    case MAIN_MENU: handle_mouse_menu_event(pp); break;
    case BEGINNER: handle_mouse_beginner_event(pp); break;
    case MEDIUM: handle_mouse_medium_event(pp); break;
    case HARD: handle_mouse_hard_event(pp); break;
    case WIN: handle_mouse_gamewin_event(pp); break;
    case GAMEOVER: handle_mouse_gameover_event(pp); break;
    case INSTRUCTIONS: handle_mouse_instruction_event(pp); break;
    case EXIT: break;
  }
}

void handle_mouse_beginner_event(struct packet pp) {
  update_cursor(pp.delta_x, pp.delta_y);

  if(pp.lb) check_mouse_click_left_beginner();
  if(pp.rb) check_mouse_click_right_beginner();
}

void handle_mouse_medium_event(struct packet pp) {
  update_cursor(pp.delta_x, pp.delta_y);

  if(pp.lb) check_mouse_click_left_medium();
  if(pp.rb) check_mouse_click_right_medium();
}

void handle_mouse_hard_event(struct packet pp) {
  update_cursor(pp.delta_x, pp.delta_y);

  if(pp.lb) check_mouse_click_left_hard();
  if(pp.rb) check_mouse_click_right_hard();
}

void handle_mouse_menu_event(struct packet pp) {
  update_cursor(pp.delta_x, pp.delta_y);
  if (pp.lb) check_mouse_clicks_menu();
}

void handle_mouse_gamewin_event(struct packet pp) {
  update_cursor(pp.delta_x, pp.delta_y);
  if (pp.lb) check_mouse_clicks_win();
}

void handle_mouse_gameover_event(struct packet pp) {
  update_cursor(pp.delta_x, pp.delta_y);
  if (pp.lb) check_mouse_clicks_finished();
}

void handle_mouse_instruction_event(struct packet pp) {
  update_cursor(pp.delta_x, pp.delta_y);
}
