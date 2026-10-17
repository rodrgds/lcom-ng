#include "timer_event_handler.h"

char board_16x16[16][16];
char board_20x16[20][16];
char board_28x20[28][20];

void handle_timer_event() {
  switch (state) {
    case MAIN_MENU: handle_timer_menu_event(); break;
    case BEGINNER: handle_timer_beginner_event(); break;
    case MEDIUM: handle_timer_medium_event(); break;
    case HARD: handle_timer_hard_event(); break;
    case INSTRUCTIONS: handle_timer_instructions_event(); break;
    case WIN: handle_timer_gamewin_event(); break;
    case GAMEOVER: handle_timer_gameover_event(); break;
    case EXIT: break;
  }
}

void handle_timer_menu_event() {
  cursor_erase();
  _main_menu();
  cursor_draw();
}

void handle_timer_beginner_event() {
  cursor_erase();
  draw_board_16x16();
  draw_selected_cell(270 + (x_matrix_kbd * 16), 262 + (y_matrix_kbd * 16));
  game_draw_time(timer);
  draw_faceRestart_16x16();
  draw_grid_16x16(board_16x16);
  if (winGame_16x16(board_16x16)) state = WIN;
  cursor_draw();
}

void handle_timer_medium_event() {
  cursor_erase();
  draw_board_20x16();
  draw_selected_cell(240 + (x_matrix_kbd * 16), 250 + (y_matrix_kbd * 16));
  game_draw_time(timer);
  draw_faceRestart_20x16();
  draw_grid_20x16(board_20x16);
  if (winGame_20x16(board_20x16)) state = WIN;
  cursor_draw();
}

void handle_timer_hard_event() {
  cursor_erase();
  draw_board_28x20();
  draw_selected_cell(152 + (x_matrix_kbd * 16), 270 + (y_matrix_kbd * 16));
  game_draw_time(timer);
  draw_faceRestart_28x20();
  draw_grid_28x20(board_28x20);
  if (winGame_28x20(board_28x20)) state = WIN;
  cursor_draw();
}

void handle_timer_instructions_event() {
  cursor_erase();
  _instructions_menu();
  cursor_draw();
}

void handle_timer_gamewin_event() {
  cursor_erase();
  draw_final_time(m1, m2, s1, s2);
  _win_menu();
  draw_final_time(m1, m2, s1, s2);
  cursor_draw();
}

void handle_timer_gameover_event() {
  cursor_erase();
  draw_final_time(m1, m2, s1, s2);
  _gameover_menu();
  draw_final_time(m1, m2, s1, s2);
  cursor_draw();
}
