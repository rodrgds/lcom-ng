#include "game_state.h"

game_state state = MAIN_MENU;
Position cursor_pos = {400, 300};
int mines, x_tiles, y_tiles;
int y_matrix, x_matrix;
int y_matrix_kbd, x_matrix_kbd;

char board_16x16[16][16];
char board_20x16[20][16];
char board_28x20[28][20];
int y_coord, x_coord;

Time* timer;
Time* initialTimer;
int m1, m2, s1, s2;


void update_cursor(uint16_t x_val, uint16_t y_val) {
  
  uint16_t new_x_pos = cursor_pos.x + x_val;
  uint16_t new_y_pos = cursor_pos.y - y_val;

  if (new_x_pos > 0 && new_x_pos < 800) cursor_pos.x = new_x_pos;
  if (new_y_pos > 0 && new_y_pos < 600) cursor_pos.y = new_y_pos;
}

void start_game_16x16() {
  initialTimer = (Time*)initTime();
  timer = (Time*)initTime();
  x_matrix_kbd = 0; 
  y_matrix_kbd = 0;

  getCurrentTime(initialTimer);
  getCurrentTime(timer);

  x_tiles = 16;
  y_tiles = 16;
  mines = 45;

  initializeBoard_16x16(board_16x16);
  placeMines_16x16(board_16x16, mines);
}

void start_game_20x16() {
  x_matrix_kbd = 0; 
  y_matrix_kbd = 0;
  initialTimer = (Time*)initTime();
  timer = (Time*)initTime();

  getCurrentTime(initialTimer);
  getCurrentTime(timer);

  x_tiles = 20;
  y_tiles = 16;
  mines = 65;
  initializeBoard_20x16(board_20x16);
  placeMines_20x16(board_20x16, mines);
}

void start_game_28x20() {
  x_matrix_kbd = 0; 
  y_matrix_kbd = 0;
  initialTimer = (Time*)initTime();
  timer = (Time*)initTime();

  getCurrentTime(initialTimer);
  getCurrentTime(timer);

  x_tiles = 28;
  y_tiles = 20;
  mines = 100;

  initializeBoard_28x20(board_28x20);
  placeMines_28x20(board_28x20, mines);
}

void cursor_erase() {
  erase_cursor(cursor_pos.x, cursor_pos.y);
}

void _main_menu() {
  draw_main_menu();
}

void cursor_draw() {
  draw_cursor(cursor_pos.x, cursor_pos.y);
}

void _draw_board() {
  draw_board_16x16();
}

void _instructions_menu() {
  draw_instructions_menu();
}

void _win_menu() {
  draw_win_menu();
}

void _gameover_menu() {
  draw_gameover_menu();
}

void check_mouse_clicks_menu() {
  if (cursor_pos.x >= 214 && cursor_pos.x <= 594) {
    if (cursor_pos.y >= 170 && cursor_pos.y <= 240){
      start_game_16x16(); 
      state = BEGINNER;
    } 
    else if (cursor_pos.y >= 251 && cursor_pos.y <= 321){
      start_game_20x16(); 
      state = MEDIUM;
    }
    else if (cursor_pos.y >= 332 && cursor_pos.y <= 402){
      start_game_28x20();
      state = HARD;
    }
    else if (cursor_pos.y >= 412 && cursor_pos.y <= 482) {
      state = INSTRUCTIONS;
    } 
    else if (cursor_pos.y >= 493 && cursor_pos.y <= 562) {
      state = EXIT;
    }
  }
}

/*--------------- BEGINNER MODE ------------------*/
void check_mouse_click_left_beginner() {
  if (cursor_pos.x >= 214 && cursor_pos.x <= 594) {
    if (cursor_pos.y >= 170 && cursor_pos.y <= 240){
      start_game_16x16();
      state = BEGINNER; 
    }
  }

  if (cursor_pos.x >= 270 && cursor_pos.x <= 533) {
    if (cursor_pos.y >= 261 && cursor_pos.y <= 518) {
      x_matrix = getMatrixIndexX(270);
      y_matrix = getMatrixIndexY(261);
      int adjMines = 0; 

      if(board_16x16[x_matrix][y_matrix] == '-') {
        adjMines = countAdjacentMines_16x16(board_16x16, x_matrix, y_matrix);
        board_16x16[x_matrix][y_matrix] = adjMines;

        if ((adjMines - '0') == 0){
          checkEmptyPlates_16x16(board_16x16, x_matrix, y_matrix);
        }
          
      }
      else if (board_16x16[x_matrix][y_matrix] == '*') {
        state = GAMEOVER;
      }
    } 
  }
}

void check_mouse_click_right_beginner() {
  if (cursor_pos.x >= 270 && cursor_pos.x <= 533) {
    if (cursor_pos.y >= 261 && cursor_pos.y <= 518) {
      x_matrix = getMatrixIndexX(270);
      y_matrix = getMatrixIndexY(261);

      if(board_16x16[x_matrix][y_matrix] == '-'){
        board_16x16[x_matrix][y_matrix] = 'f';
      }

      else if(board_16x16[x_matrix][y_matrix] == '*'){
        board_16x16[x_matrix][y_matrix] = 'F';
      }

      else if(board_16x16[x_matrix][y_matrix] == 'f'){
        board_16x16[x_matrix][y_matrix] = '-';
      }

      else if(board_16x16[x_matrix][y_matrix] == 'F'){
        board_16x16[x_matrix][y_matrix] = '*';
      }
    }
  }
}

/*--------------- MEDIUM MODE ------------------*/
void check_mouse_click_left_medium() {
  if (cursor_pos.x >= 359 && cursor_pos.x <= 439) {
    if (cursor_pos.y >= 113 && cursor_pos.y <= 194){
      start_game_20x16();
      state = MEDIUM; 
    }
  }

  if (cursor_pos.x >= 240 && cursor_pos.x <= 561) {
    if (cursor_pos.y >= 250 && cursor_pos.y <= 506) {
      x_matrix = getMatrixIndexX(240);
      y_matrix = getMatrixIndexY(250);
      int adjMines = 0; 

      if(board_20x16[x_matrix][y_matrix] == '-') {
        adjMines = countAdjacentMines_20x16(board_20x16, x_matrix, y_matrix);
        board_20x16[x_matrix][y_matrix] = adjMines;

        if ((adjMines - '0') == 0){
          checkEmptyPlates_20x16(board_20x16, x_matrix, y_matrix);
        }
          
      }

      else if (board_20x16[x_matrix][y_matrix] == '*') {
        state = GAMEOVER;
      }
    } 
  }
}

void check_mouse_click_right_medium() {
  if (cursor_pos.x >= 240 && cursor_pos.x <= 561) {
    if (cursor_pos.y >= 250 && cursor_pos.y <= 506) {
      x_matrix = getMatrixIndexX(240);
      y_matrix = getMatrixIndexY(250);

      if(board_20x16[x_matrix][y_matrix] == '-'){
        board_20x16[x_matrix][y_matrix] = 'f';
      }

      else if(board_20x16[x_matrix][y_matrix] == '*'){
        board_20x16[x_matrix][y_matrix] = 'F';
      }

      else if(board_20x16[x_matrix][y_matrix] == 'f'){
        board_20x16[x_matrix][y_matrix] = '-';
      }

      else if(board_20x16[x_matrix][y_matrix] == 'F'){
        board_20x16[x_matrix][y_matrix] = '*';
      }
    }
  }
}

/*--------------- HARD MODE ------------------*/
void check_mouse_click_left_hard() {
  if (cursor_pos.x >= 336 && cursor_pos.x <= 416) {
    if (cursor_pos.y >= 91 && cursor_pos.y <= 172){
      start_game_28x20();
      state = HARD; 
    }
  }

  if (cursor_pos.x >= 152 && cursor_pos.x <= 600) {
    if (cursor_pos.y >= 270 && cursor_pos.y <= 590) {
      x_matrix = getMatrixIndexX(152);
      y_matrix = getMatrixIndexY(270);
      int adjMines = 0; 

      if(board_28x20[x_matrix][y_matrix] == '-') {
        adjMines = countAdjacentMines_28x20(board_28x20, x_matrix, y_matrix);
        board_28x20[x_matrix][y_matrix] = adjMines;

        if ((adjMines - '0') == 0){
          checkEmptyPlates_28x20(board_28x20, x_matrix, y_matrix);
        }
          
      }

      else if (board_28x20[x_matrix][y_matrix] == '*') {
        state = GAMEOVER;
      }
    } 
  }
}

void check_mouse_click_right_hard() {
  if (cursor_pos.x >= 152 && cursor_pos.x <= 600) {
    if (cursor_pos.y >= 270 && cursor_pos.y <= 590) {
      x_matrix = getMatrixIndexX(152);
      y_matrix = getMatrixIndexY(270);

      if(board_28x20[x_matrix][y_matrix] == '-'){
        board_28x20[x_matrix][y_matrix] = 'f';
      }

      else if(board_28x20[x_matrix][y_matrix] == '*'){
        board_28x20[x_matrix][y_matrix] = 'F';
      }

      else if(board_28x20[x_matrix][y_matrix] == 'f'){
        board_28x20[x_matrix][y_matrix] = '-';
      }

      else if(board_28x20[x_matrix][y_matrix] == 'F'){
        board_28x20[x_matrix][y_matrix] = '*';
      }
    }
  }
}

void check_mouse_clicks_win() {
  state = MAIN_MENU;
}

void check_mouse_clicks_finished() {
  state = MAIN_MENU;
}

void game_draw_time(Time* timer) {
  int timesec, inttimesec, realTime, realTimeSec, realTimeMin;

  getCurrentTime(timer);

  timesec = (((timer->hour * 60) + timer->minute) * 60) + timer->second;
  inttimesec = (((initialTimer->hour * 60) + initialTimer->minute) * 60) + initialTimer->second;

  realTime = timesec - inttimesec;
  
  realTimeSec = realTime % 60;
  realTimeMin = realTime / 60;

  m1 = realTimeMin / 10;
  m2 = realTimeMin % 10;
  s1 = realTimeSec / 10;
  s2 = realTimeSec % 10;
  switch (state) {
    case BEGINNER:
      draw_number(m1, 274, 172);
      draw_number(m2, 289, 172);
      draw_colon(302, 172);
      draw_number(s1, 319, 172);
      draw_number(s2, 334, 172);
      break;

    case MEDIUM:
      draw_number(m1, 255, 147);
      draw_number(m2, 270, 147);
      draw_colon(282, 147);
      draw_number(s1, 300, 147);
      draw_number(s2, 315, 147);
      break;
    
    case HARD:
      draw_number(m1, 190, 120);
      draw_number(m2, 205, 120);
      draw_colon(217, 120);
      draw_number(s1, 235, 120);
      draw_number(s2, 250, 120);
      break;
    
    default:
      break;
  }
  
}

void check_keyboard_w() {
  if (y_matrix_kbd == 0) y_matrix_kbd = 0;
  else y_matrix_kbd--;
}

void check_keyboard_a() {
  if (x_matrix_kbd == 0) x_matrix_kbd = 0;
  else x_matrix_kbd--;
}

void check_keyboard_s_16x16() {
  if (y_matrix_kbd == 15) y_matrix_kbd = 15;
  else y_matrix_kbd++;
}

void check_keyboard_s_20x16() {
  if (y_matrix_kbd == 15) y_matrix_kbd = 15;
  else y_matrix_kbd++;
}

void check_keyboard_s_28x20() {
  if (y_matrix_kbd == 19) y_matrix_kbd = 19;
  else y_matrix_kbd++;
}

void check_keyboard_d_16x16() {
  if (x_matrix_kbd == 15) x_matrix_kbd = 15;
  else x_matrix_kbd++;
}

void check_keyboard_d_20x16() {
  if (x_matrix_kbd == 19) x_matrix_kbd = 19;
  else x_matrix_kbd++;
}

void check_keyboard_d_28x20() {
  if (x_matrix_kbd == 27) x_matrix_kbd = 27;
  else x_matrix_kbd++;
}

void check_keyboard_enter_16x16() {
  char adjMines;
  if (board_16x16[x_matrix_kbd][y_matrix_kbd] == '-') {
    adjMines = countAdjacentMines_16x16(board_16x16, x_matrix_kbd, y_matrix_kbd);
    board_16x16[x_matrix_kbd][y_matrix_kbd] = adjMines;

    if ((adjMines) == '0') {
      checkEmptyPlates_16x16(board_16x16, x_matrix_kbd, y_matrix_kbd);
    }   
  }

  else if (board_16x16[x_matrix_kbd][y_matrix_kbd] == '*') {
    state = GAMEOVER;
  }
}

void check_keyboard_enter_20x16() {
  char adjMines;
  if (board_20x16[x_matrix_kbd][y_matrix_kbd] == '-') {
    adjMines = countAdjacentMines_20x16(board_20x16, x_matrix_kbd, y_matrix_kbd);
    board_20x16[x_matrix_kbd][y_matrix_kbd] = adjMines;

    if ((adjMines) == '0') {
      checkEmptyPlates_20x16(board_20x16, x_matrix_kbd, y_matrix_kbd);
    }
  }

  else if (board_20x16[x_matrix_kbd][y_matrix_kbd] == '*') {
    state = GAMEOVER;
  }
}

void check_keyboard_enter_28x20() {
  char adjMines;
  if (board_28x20[x_matrix_kbd][y_matrix_kbd] == '-') {
    adjMines = countAdjacentMines_28x20(board_28x20, x_matrix_kbd, y_matrix_kbd);
    board_28x20[x_matrix_kbd][y_matrix_kbd] = adjMines;

    if ((adjMines) == '0') {
      checkEmptyPlates_28x20(board_28x20, x_matrix_kbd, y_matrix_kbd);
    }   
  }

  else if (board_28x20[x_matrix_kbd][y_matrix_kbd] == '*') {
    state = GAMEOVER;
  }
}

void check_keyboard_f_16x16() {
  if (board_16x16[x_matrix_kbd][y_matrix_kbd] == '-'){
    board_16x16[x_matrix_kbd][y_matrix_kbd] = 'f';
  }

  else if (board_16x16[x_matrix_kbd][y_matrix_kbd] == '*') {
    board_16x16[x_matrix_kbd][y_matrix_kbd] = 'F';
  }

  else if (board_16x16[x_matrix_kbd][y_matrix_kbd] == 'f') {
    board_16x16[x_matrix_kbd][y_matrix_kbd] = '-';
  }

  else if (board_16x16[x_matrix_kbd][y_matrix_kbd] == 'F') {
    board_16x16[x_matrix_kbd][y_matrix_kbd] = '*';
  }
}

void check_keyboard_f_20x16() {
  if (board_20x16[x_matrix_kbd][y_matrix_kbd] == '-'){
    board_20x16[x_matrix_kbd][y_matrix_kbd] = 'f';
  }

  else if (board_20x16[x_matrix_kbd][y_matrix_kbd] == '*') {
    board_20x16[x_matrix_kbd][y_matrix_kbd] = 'F';
  }

  else if (board_20x16[x_matrix_kbd][y_matrix_kbd] == 'f') {
    board_20x16[x_matrix_kbd][y_matrix_kbd] = '-';
  }

  else if (board_20x16[x_matrix_kbd][y_matrix_kbd] == 'F') {
    board_20x16[x_matrix_kbd][y_matrix_kbd] = '*';
  }
}

void check_keyboard_f_28x20() {
  if (board_28x20[x_matrix_kbd][y_matrix_kbd] == '-'){
    board_28x20[x_matrix_kbd][y_matrix_kbd] = 'f';
  }

  else if (board_28x20[x_matrix_kbd][y_matrix_kbd] == '*') {
    board_28x20[x_matrix_kbd][y_matrix_kbd] = 'F';
  }

  else if (board_28x20[x_matrix_kbd][y_matrix_kbd] == 'f') {
    board_28x20[x_matrix_kbd][y_matrix_kbd] = '-';
  }

  else if (board_28x20[x_matrix_kbd][y_matrix_kbd] == 'F') {
    board_28x20[x_matrix_kbd][y_matrix_kbd] = '*';
  }
}
