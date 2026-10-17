// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>

// header files created by us

#include "game.h"
#include "keys.h"
#include "vbe_graphics.h"

#include "xpm/numbers/eight.xpm"
#include "xpm/numbers/five.xpm"
#include "xpm/numbers/four.xpm"
#include "xpm/numbers/nine.xpm"
#include "xpm/numbers/one.xpm"
#include "xpm/numbers/seven.xpm"
#include "xpm/numbers/six.xpm"
#include "xpm/numbers/three.xpm"
#include "xpm/numbers/two.xpm"
#include "xpm/numbers/zero.xpm"

#include "xpm/game/score_t.xpm"
#include "xpm/game/tetris_t.xpm"
#include "xpm/game/game_over.xpm"

xpm_image_t tetris_t;

uint16_t piece_pos_x = 50;
uint16_t piece_pos_y = 50;

uint8_t x_move_qty = 10;
uint8_t y_move_qty = 10;

xpm_image_t score_xpm;
int score_initial_x = 500;
int score_initial_y = 150;

xpm_image_t game_over;

xpm_image_t zero, one, two,
  three, four, five,
  six, seven, eight, nine;

extern uint8_t scancode;
extern GameState game_state;
int score = 0;

//
#define AMAKE 0x1E
#define DMAKE 0x20
#define ABREAK 0x9E
#define DBREAK 0xA0
#define WBREAK 0x91
#define SBREAK 0x9F
//

void load_score_xpm() {
  score_xpm.bytes = xpm_load(score_t_xpm, XPM_8_8_8, &score_xpm);
  zero.bytes = xpm_load(zero_xpm, XPM_8_8_8, &zero);
  one.bytes = xpm_load(one_xpm, XPM_8_8_8, &one);
  two.bytes = xpm_load(two_xpm, XPM_8_8_8, &two);
  three.bytes = xpm_load(three_xpm, XPM_8_8_8, &three);
  four.bytes = xpm_load(four_xpm, XPM_8_8_8, &four);
  five.bytes = xpm_load(five_xpm, XPM_8_8_8, &five);
  six.bytes = xpm_load(six_xpm, XPM_8_8_8, &six);
  seven.bytes = xpm_load(seven_xpm, XPM_8_8_8, &seven);
  eight.bytes = xpm_load(eight_xpm, XPM_8_8_8, &eight);
  nine.bytes = xpm_load(nine_xpm, XPM_8_8_8, &nine);
}

#define FIRST_DIGIT_X 585
#define DIGIT_Y 235
#define SECOND_DIGIT_X 600

void write_score() {
  int digits[2];
  if (score == 0) {
    draw_xpm(&zero, FIRST_DIGIT_X, DIGIT_Y);
    draw_xpm(&zero, SECOND_DIGIT_X, DIGIT_Y);
    return;
  }
  else if (score < 10) {
    digits[0] = 0;
    digits[1] = score;
    draw_xpm(&zero, FIRST_DIGIT_X, DIGIT_Y);
    if (digits[1] == 0) {
      draw_xpm(&zero, SECOND_DIGIT_X, DIGIT_Y);
    }
    else if (digits[1] == 1) {
      draw_xpm(&one, SECOND_DIGIT_X, DIGIT_Y);
    }
    else if (digits[1] == 2) {
      draw_xpm(&two, SECOND_DIGIT_X, DIGIT_Y);
    }
    else if (digits[1] == 3) {
      draw_xpm(&three, SECOND_DIGIT_X, DIGIT_Y);
    }
    else if (digits[1] == 4) {
      draw_xpm(&four, SECOND_DIGIT_X, DIGIT_Y);
    }
    else if (digits[1] == 5) {
      draw_xpm(&five, SECOND_DIGIT_X, DIGIT_Y);
    }
    else if (digits[1] == 6) {
      draw_xpm(&six, SECOND_DIGIT_X, DIGIT_Y);
    }
    else if (digits[1] == 7) {
      draw_xpm(&seven, SECOND_DIGIT_X, DIGIT_Y);
    }
    else if (digits[1] == 8) {
      draw_xpm(&eight, SECOND_DIGIT_X, DIGIT_Y);
    }
    else if (digits[1] == 9) {
      draw_xpm(&nine, SECOND_DIGIT_X, DIGIT_Y);
    }
  }
  else {
    digits[0] = score / 10;
    digits[1] = score % 10;
    if (digits[1] == 0) {
      draw_xpm(&zero, SECOND_DIGIT_X, DIGIT_Y);
    }
    else if (digits[1] == 1) {
      draw_xpm(&one, SECOND_DIGIT_X, DIGIT_Y);
    }
    else if (digits[1] == 2) {
      draw_xpm(&two, SECOND_DIGIT_X, DIGIT_Y);
    }
    else if (digits[1] == 3) {
      draw_xpm(&three, SECOND_DIGIT_X, DIGIT_Y);
    }
    else if (digits[1] == 4) {
      draw_xpm(&four, SECOND_DIGIT_X, DIGIT_Y);
    }
    else if (digits[1] == 5) {
      draw_xpm(&five, SECOND_DIGIT_X, DIGIT_Y);
    }
    else if (digits[1] == 6) {
      draw_xpm(&six, SECOND_DIGIT_X, DIGIT_Y);
    }
    else if (digits[1] == 7) {
      draw_xpm(&seven, SECOND_DIGIT_X, DIGIT_Y);
    }
    else if (digits[1] == 8) {
      draw_xpm(&eight, SECOND_DIGIT_X, DIGIT_Y);
    }
    else if (digits[1] == 9) {
      draw_xpm(&nine, SECOND_DIGIT_X, DIGIT_Y);
    }
    if (digits[0] == 1) {
      draw_xpm(&one, FIRST_DIGIT_X, DIGIT_Y);
    }
    else if (digits[0] == 2) {
      draw_xpm(&two, FIRST_DIGIT_X, DIGIT_Y);
    }
    else if (digits[0] == 3) {
      draw_xpm(&three, FIRST_DIGIT_X, DIGIT_Y);
    }
    else if (digits[0] == 4) {
      draw_xpm(&four, FIRST_DIGIT_X, DIGIT_Y);
    }
    else if (digits[0] == 5) {
      draw_xpm(&five, FIRST_DIGIT_X, DIGIT_Y);
    }
    else if (digits[0] == 6) {
      draw_xpm(&six, FIRST_DIGIT_X, DIGIT_Y);
    }
    else if (digits[0] == 7) {
      draw_xpm(&seven, FIRST_DIGIT_X, DIGIT_Y);
    }
    else if (digits[0] == 8) {
      draw_xpm(&eight, FIRST_DIGIT_X, DIGIT_Y);
    }
    else if (digits[0] == 9) {
      draw_xpm(&nine, FIRST_DIGIT_X, DIGIT_Y);
    }
  }
}

void draw_score() {
  draw_xpm(&score_xpm, score_initial_x, score_initial_y);
}

void play_game(Piece *piece) {
  Board *board = piece->board;
  if (!update_movement(piece, DOWN)) {

    if (check_gameover(piece)) {
      game_state = GAME_OVER;
    }
    if(check_full_line(piece)) {
      move_down_board(piece->board);
      score++;
    }
    if (game_state == GAME) {
      Piece *next_piece = new_piece(board);
      *piece = *next_piece;
      delete_piece(next_piece);
    }
  }
  if (game_state == GAME) {
    draw_board(piece->board);
    write_score();
  }
}

void movement_input(uint8_t *scancode, Piece *piece) {
  switch (*scancode) {
    case WBREAK:
      printf("W pressed\n");
      update_movement(piece, ROTATE);
      *scancode = 0;
      break;
    case SBREAK:
      printf("S pressed\n");
      update_movement(piece, DOWN);
      *scancode = 0;
      break;
    case DBREAK:
      printf("D pressed\n");
      update_movement(piece, RIGHT);
      break;
    case ABREAK:
      printf("A pressed\n");
      update_movement(piece, LEFT);
      break;
    default:
      break;
  }
}

bool check_full_lines(Board *board) {
  bool free_cell = false;
  for (int i = 190; i < 200; i++) {
    if (board->cells[i].color == BLACK) {
      free_cell = true;
    }
  }
  return !free_cell;
}

void move_down_board(Board *board) {
  for (int i = 190; i > 10; i--) {
    board->cells[i].color = board->cells[i - 10].color;
  }
  for (int i = 0; i < 10; i++) {
    board->cells[i].color = BLACK;
  }
}

bool check_gameover(Piece *piece) {
  for (int i = 0; i < 10; i++) {
    if (piece->board->cells[i].color != BLACK)
      return true;
  }
  return false;
}

bool check_full_line(Piece *piece) {
  for (int i = 190; i < 200; i++) {
    if (piece->board->cells[i].color == BLACK) {
      return false;
    }
  }
  return true;
}

void load_game_over(){
 xpm_load(game_over_t_xpm,XPM_8_8_8,&game_over);
}

void write_game_over(){
  draw_xpm(&game_over,0,0);
}
