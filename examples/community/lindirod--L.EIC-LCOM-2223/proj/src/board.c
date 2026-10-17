// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>

#include <lcom/proj.h>

#include <stdint.h>
#include <stdio.h>

///////////////////////////////////////////////////

#include "board.h"

uint16_t initial_x = 200;
uint16_t initial_y = 150;

void(draw_board)(Board *bd) {
  for (int i = 0; i < 200; i++) {
    // position in board to position on screen
    uint16_t xp, yp;
    xp = SIZE * bd->cells[i].x + initial_x;
    yp = SIZE * bd->cells[i].y + initial_y;
    vg_draw_rectangle(xp, yp, SIZE, SIZE, bd->cells[i].color);
  }
  // draw borders
  vg_draw_vline(initial_x, initial_y, 20 * SIZE, 0xffffff);
  vg_draw_vline(initial_x + (10 * SIZE), initial_y, 20 * SIZE, 0xffffff);
  vg_draw_hline(initial_x, initial_y, 10 * SIZE, 0xffffff);
  vg_draw_hline(initial_x, initial_y + (20 * SIZE), 10 * SIZE, 0xffffff);
}

Board *new_board() {
  Board *board = calloc(1, sizeof(Board));
  board->width = 10;
  board->height = 20;

  int n = 0;

  for (size_t i = 0; i < board->height; i++)
    for (size_t j = 0; j < board->width; j++) {
      board->cells[n].y = i;
      board->cells[n].x = j;
      board->cells[n].color = BLACK;
      n++;
    }

  return board;
}

Piece *new_piece(Board *board) {
  Piece *piece = malloc(sizeof(Piece));
  piece->piece_shape = rand() % 5;
  uint32_t possible_colors[6] = {YELLOW, PURPLE, GREEN, RED, BLUE, ORANGE};
  int random_idx = rand() % 6;
  piece->color = possible_colors[random_idx];
  piece->board = board;
  piece->rot = 1;

  if (piece->piece_shape == T) {
    piece->board_index[0] = 4;
    piece->board_index[1] = 5;
    piece->board_index[2] = 6;
    piece->board_index[3] = 15;
  }
  else if (piece->piece_shape == I) {
    piece->board_index[0] = 3;
    piece->board_index[1] = 4;
    piece->board_index[2] = 5;
    piece->board_index[3] = 6;
  }
  else if (piece->piece_shape == Z) {
    piece->board_index[0] = 3;
    piece->board_index[1] = 4;
    piece->board_index[2] = 14;
    piece->board_index[3] = 15;
  }
  else if (piece->piece_shape == O) {
    piece->board_index[0] = 4;
    piece->board_index[1] = 5;
    piece->board_index[2] = 14;
    piece->board_index[3] = 15;
  }
  else if(piece->piece_shape == L){
    piece->board_index[0] = 13;
    piece->board_index[1] = 14;
    piece->board_index[2] = 15;
    piece->board_index[3] = 5;
  }

  for (size_t i = 0; i < 4; i++) {
    piece->board->cells[piece->board_index[i]].color = piece->color;
  }

  return piece;
}

void delete_piece(Piece *piece) {
  free(piece);
}

void delete_board(Board *board) {
  free(board);
}
