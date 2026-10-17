// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>

#include <lcom/proj.h>

#include <stdint.h>
#include <stdio.h>

///////////////////////////////////////////////////

#include "board.h"
#include "piece_move.h"

bool update_movement(Piece *piece, enum movement_type move) {
  switch (move) {
    case ROTATE:
      if (!rotate_piece(piece)) {
        printf("Can't rotate piece!\n");
        return false;
      }
      else {
        return true;
      }
    case DOWN:
      if (!move_down(piece)) {
        printf("Can't move down!\n");
        return false;
      }
      else {
        return true;
      }
    case RIGHT:
      if (!move_right(piece)) {
        printf("Can't move right\n");
        return false;
      }
      else {
        return true;
      }
    case LEFT:
      if (!move_left(piece)) {
        printf("Can't move left\n");
      }
    default:
      return false;
  }
  return true;
}

bool rotate_piece(Piece *piece) {
  uint16_t rotated_idx[4];

  //------------------------ T PIECE ROTATION --------------------------------//
  if (piece->piece_shape == T) {
    if (piece->rot == 1) { // rotate 1 -> 2 , T -> -|
      rotated_idx[0] = piece->board_index[1] - 10;
      rotated_idx[1] = piece->board_index[1];
      rotated_idx[2] = piece->board_index[3];
      rotated_idx[3] = piece->board_index[0];

      // printf("T piece rotation 1\n");

      if (piece->board_index[1] < 10) {
        return false;
      }
    }
    else if (piece->rot == 2) { // rotate -| -> _|_
      rotated_idx[0] = piece->board_index[2] + 1;
      rotated_idx[1] = piece->board_index[2];
      rotated_idx[2] = piece->board_index[2] - 1;
      rotated_idx[3] = piece->board_index[1];

      if (piece->board->cells[rotated_idx[0]].color != BLACK || piece->board->cells[rotated_idx[2]].color != BLACK) {
        return false;
      }
    }
    else if (piece->rot == 3) { // rotate _|_ -> |-
      rotated_idx[0] = piece->board_index[1];
      rotated_idx[1] = piece->board_index[3];
      rotated_idx[2] = piece->board_index[3] - 10;
      rotated_idx[3] = piece->board_index[3] + 1;

      if (piece->board->cells[rotated_idx[3]].color != BLACK || piece->board_index[3] < 10) {
        return false;
      }
    }
    else if (piece->rot == 4) { // rotate |- -> T
      rotated_idx[0] = piece->board_index[1] - 1;
      rotated_idx[1] = piece->board_index[1];
      rotated_idx[2] = piece->board_index[1] + 1;
      rotated_idx[3] = piece->board_index[0];

      if (piece->board->cells[rotated_idx[0]].color != BLACK) {
        return false;
      }
    }
    else {
      return false;
    }
  }

  //------------------------ I PIECE ROTATION --------------------------------//

  else if (piece->piece_shape == I) {
    if (piece->rot == 1) { // rotate 1 -> 2 , __ -> |
      rotated_idx[0] = piece->board_index[2] - 20;
      rotated_idx[1] = piece->board_index[2] - 10;
      rotated_idx[2] = piece->board_index[2];
      rotated_idx[3] = piece->board_index[2] + 10;

      // printf("T piece rotation 1\n");

      if (piece->board_index[0] < 20 || piece->board_index[2] > 189) {
        return false;
      }
      else if (piece->board->cells[rotated_idx[0]].color != BLACK || piece->board->cells[rotated_idx[1]].color != BLACK || piece->board->cells[rotated_idx[3]].color != BLACK) {
        return false;
      }
    }
    else if (piece->rot == 2) { // rotate | -> __
      rotated_idx[0] = piece->board_index[2] + 2;
      rotated_idx[1] = piece->board_index[2] + 1;
      rotated_idx[2] = piece->board_index[2];
      rotated_idx[3] = piece->board_index[2] - 1;

      if (piece->board_index[2] % 10 == 9 || piece->board_index[2] % 10 == 8 || piece->board_index[2] % 10 == 0) {
        return false;
      }

      else if (piece->board->cells[rotated_idx[0]].color != BLACK || piece->board->cells[rotated_idx[1]].color != BLACK || piece->board->cells[rotated_idx[3]].color != BLACK) {
        return false;
      }
    }
    else if (piece->rot == 3) { // rotate __ -> |
      rotated_idx[0] = piece->board_index[2] + 10;
      rotated_idx[1] = piece->board_index[2];
      rotated_idx[2] = piece->board_index[2] - 10;
      rotated_idx[3] = piece->board_index[2] - 20;

      if (piece->board_index[2] < 20 || piece->board_index[2] > 189) {
        return false;
      }
      else if (piece->board->cells[rotated_idx[0]].color != BLACK || piece->board->cells[rotated_idx[2]].color != BLACK || piece->board->cells[rotated_idx[3]].color != BLACK) {
        return false;
      }
    }
    else if (piece->rot == 4) { // rotate | -> __
      rotated_idx[0] = piece->board_index[1] - 1;
      rotated_idx[1] = piece->board_index[1];
      rotated_idx[2] = piece->board_index[1] + 1;
      rotated_idx[3] = piece->board_index[1] + 2;

      if (piece->board_index[1] % 10 == 9 || piece->board_index[1] % 10 == 8 || piece->board_index[1] % 10 == 0) {
        return false;
      }

      else if (piece->board->cells[rotated_idx[0]].color != BLACK || piece->board->cells[rotated_idx[2]].color != BLACK || piece->board->cells[rotated_idx[3]].color != BLACK) {
        return false;
      }
    }
    else {
      return false;
    }
  }
  //------------------------ Z PIECE ROTATION --------------------------------//

  else if (piece->piece_shape == Z) {
    if (piece->rot == 1 || piece->rot == 3) { // Z
      rotated_idx[0] = piece->board_index[3] - 20;
      rotated_idx[1] = piece->board_index[3] - 10;
      rotated_idx[2] = piece->board_index[1];
      rotated_idx[3] = piece->board_index[2];

      if (piece->board_index[1] < 10) {
        return false;
      }
      else if (piece->board->cells[rotated_idx[0]].color != BLACK || piece->board->cells[rotated_idx[1]].color != BLACK) {
        return false;
      }
    }
    else if (piece->rot == 2 || piece->rot == 4) {
      rotated_idx[0] = piece->board_index[2] - 1;
      rotated_idx[1] = piece->board_index[2];
      rotated_idx[2] = piece->board_index[3];
      rotated_idx[3] = piece->board_index[3] + 1;

      if (piece->board_index[2] % 10 == 0) {
        return false;
      }

      else if (piece->board->cells[rotated_idx[0]].color != BLACK || piece->board->cells[rotated_idx[3]].color != BLACK) {
        return false;
      }
    }
    else {
      return false;
    }
  }

  //------------------------ O PIECE ROTATION --------------------------------//

  else if (piece->piece_shape == O) {
    return true;
  }

  //------------------------ L PIECE ROTATION --------------------------------//

  else if (piece->piece_shape == L) {
    if (piece->rot == 1) {
      rotated_idx[0] = piece->board_index[1] - 10;
      rotated_idx[1] = piece->board_index[1];
      rotated_idx[2] = piece->board_index[1] + 10;
      rotated_idx[3] = piece->board_index[2] + 10;

      if (piece->board_index[1] > 189) {
        return false;
      }
      else if (piece->board->cells[rotated_idx[0]].color != BLACK || piece->board->cells[rotated_idx[2]].color != BLACK || piece->board->cells[rotated_idx[3]].color != BLACK) {
        return false;
      }
    }
    else if (piece->rot == 2) {
      rotated_idx[0] = piece->board_index[1] + 1;
      rotated_idx[1] = piece->board_index[1];
      rotated_idx[2] = piece->board_index[1] - 1;
      rotated_idx[3] = piece->board_index[2] - 1;

      if (piece->board_index[1] % 10 == 0) {
        return false;
      }

      else if (piece->board->cells[rotated_idx[0]].color != BLACK || piece->board->cells[rotated_idx[2]].color != BLACK || piece->board->cells[rotated_idx[3]].color != BLACK) {
        return false;
      }
    }
    else if (piece->rot == 3) {
      rotated_idx[0] = piece->board_index[1] + 10;
      rotated_idx[1] = piece->board_index[1];
      rotated_idx[2] = piece->board_index[1] - 10;
      rotated_idx[3] = piece->board_index[2] - 10;

      if (piece->board_index[1] < 10) {
        return false;
      }
      else if (piece->board->cells[rotated_idx[0]].color != BLACK || piece->board->cells[rotated_idx[2]].color != BLACK || piece->board->cells[rotated_idx[3]].color != BLACK) {
        return false;
      }
    }
    else if (piece->rot == 4) {
      rotated_idx[0] = piece->board_index[1] - 1;
      rotated_idx[1] = piece->board_index[1];
      rotated_idx[2] = piece->board_index[1] + 1;
      rotated_idx[3] = piece->board_index[2] + 1;

      if (piece->board_index[1] % 10 == 9) {
        return false;
      }

      else if (piece->board->cells[rotated_idx[0]].color != BLACK || piece->board->cells[rotated_idx[2]].color != BLACK || piece->board->cells[rotated_idx[3]].color != BLACK) {
        return false;
      }
    }
    else {
      return false;
    }
  }

  //------------------------- DEFAULT CASE ---------------------------------//
  else {
    return false;
  }

  //-------------------------CHANGE PIECE IN BOARD-------------------------//
  for (int i = 0; i < 4; i++) piece->board->cells[piece->board_index[i]].color = BLACK;

  for (int i = 0; i < 4; i++) {
    piece->board_index[i] = rotated_idx[i];
    piece->board->cells[piece->board_index[i]].color = piece->color;
  }

  //--------------------------- CHANGE ROT VARIABLE -----------------------//
  if (piece->rot == 4) {
    piece->rot = 1;
  }
  else {
    piece->rot++;
  }

  return true;
}

bool move_down(Piece *piece) {
  uint16_t down_idx[4];

  //------------------------ T PIECE DOWN --------------------------------//
  if (piece->piece_shape == T) {
    if (piece->rot == 1) { // ROT = T
      for (int i = 0; i < 4; i++) {
        down_idx[i] = piece->board_index[i] + 10;
      }
      if (down_idx[3] > BOARD_MAX_IDX) {
        return false;
      }
      else if (piece->board->cells[down_idx[0]].color != BLACK || piece->board->cells[down_idx[3]].color != BLACK || piece->board->cells[down_idx[2]].color != BLACK) {
        return false;
      }
    }
    else if (piece->rot == 2) { // ROT = -|
      for (int i = 0; i < 4; i++) {
        down_idx[i] = piece->board_index[i] + 10;
      }
      if (down_idx[2] > BOARD_MAX_IDX) {
        return false;
      }
      else if (piece->board->cells[down_idx[3]].color != BLACK || piece->board->cells[down_idx[2]].color != BLACK) {
        return false;
      }
    }
    else if (piece->rot == 3) { // ROT = _|_
      printf("T piece rot 3\n");
      for (int i = 0; i < 4; i++) {
        down_idx[i] = piece->board_index[i] + 10;
      }
      if (down_idx[1] > BOARD_MAX_IDX) {
        return false;
      }
      else if (piece->board->cells[down_idx[0]].color != BLACK || piece->board->cells[down_idx[1]].color != BLACK || piece->board->cells[down_idx[2]].color != BLACK) {
        return false;
      }
    }
    else if (piece->rot == 4) { // ROT = |-
      for (int i = 0; i < 4; i++) {
        down_idx[i] = piece->board_index[i] + 10;
      }
      if (down_idx[0] > BOARD_MAX_IDX) {
        return false;
      }
      else if (piece->board->cells[down_idx[0]].color != BLACK || piece->board->cells[down_idx[3]].color != BLACK) {
        return false;
      }
    }
    else {
      return false;
    }
  }
  //------------------------------ I PIECE MOVE DOWN ------------------------------------//
  else if (piece->piece_shape == I) {
    if (piece->rot == 1) { // ROT = __
      for (int i = 0; i < 4; i++) {
        down_idx[i] = piece->board_index[i] + 10;
      }
      if (down_idx[1] > BOARD_MAX_IDX) {
        return false;
      }
      else if (piece->board->cells[down_idx[0]].color != BLACK || piece->board->cells[down_idx[1]].color != BLACK || piece->board->cells[down_idx[2]].color != BLACK || piece->board->cells[down_idx[3]].color != BLACK) {
        return false;
      }
    }
    else if (piece->rot == 2) { // ROT = |
      for (int i = 0; i < 4; i++) {
        down_idx[i] = piece->board_index[i] + 10;
      }
      if (down_idx[3] > BOARD_MAX_IDX) {
        return false;
      }
      else if (piece->board->cells[down_idx[3]].color != BLACK) {
        return false;
      }
    }
    else if (piece->rot == 3) { // ROT = __
      for (int i = 0; i < 4; i++) {
        down_idx[i] = piece->board_index[i] + 10;
      }
      if (down_idx[2] > BOARD_MAX_IDX) {
        return false;
      }
      else if (piece->board->cells[down_idx[0]].color != BLACK || piece->board->cells[down_idx[1]].color != BLACK || piece->board->cells[down_idx[2]].color != BLACK || piece->board->cells[down_idx[3]].color != BLACK) {
        return false;
      }
    }
    else if (piece->rot == 4) { // ROT = |
      for (int i = 0; i < 4; i++) {
        down_idx[i] = piece->board_index[i] + 10;
      }
      if (down_idx[0] > BOARD_MAX_IDX) {
        return false;
      }
      else if (piece->board->cells[down_idx[0]].color != BLACK) {
        return false;
      }
    }
    else {
      return false;
    }
  }
  //------------------------------ Z PIECE MOVE DOWN ------------------------------------//
  else if (piece->piece_shape == Z) {
    if (piece->rot == 1 || piece->rot == 3) {
      for (int i = 0; i < 4; i++) {
        down_idx[i] = piece->board_index[i] + 10;
      }
      if (down_idx[2] > BOARD_MAX_IDX) {
        return false;
      }
      else if (piece->board->cells[down_idx[0]].color != BLACK || piece->board->cells[down_idx[2]].color != BLACK || piece->board->cells[down_idx[3]].color != BLACK) {
        return false;
      }
    }
    else if (piece->rot == 2 || piece->rot == 4) { // ROT = |
      for (int i = 0; i < 4; i++) {
        down_idx[i] = piece->board_index[i] + 10;
      }
      if (down_idx[3] > BOARD_MAX_IDX) {
        return false;
      }
      else if (piece->board->cells[down_idx[1]].color != BLACK || piece->board->cells[down_idx[3]].color != BLACK) {
        return false;
      }
    }
    else {
      return false;
    }
  }
  //------------------------------ O PIECE MOVE DOWN ------------------------------------//
  else if (piece->piece_shape == O) {
    if (piece->rot == 1 || piece->rot == 2 || piece->rot == 3 || piece->rot == 4) {
      for (int i = 0; i < 4; i++) {
        down_idx[i] = piece->board_index[i] + 10;
      }
      if (down_idx[2] > BOARD_MAX_IDX) {
        return false;
      }
      else if (piece->board->cells[down_idx[2]].color != BLACK || piece->board->cells[down_idx[3]].color != BLACK) {
        return false;
      }
    }
    else {
      return false;
    }
  }
  //------------------------------ L PIECE MOVE DOWN ------------------------------------//
  else if (piece->piece_shape == L) {
    if (piece->rot == 1) {
      for (int i = 0; i < 4; i++) {
        down_idx[i] = piece->board_index[i] + 10;
      }
      if (down_idx[1] > BOARD_MAX_IDX) {
        return false;
      }
      else if (piece->board->cells[down_idx[0]].color != BLACK || piece->board->cells[down_idx[1]].color != BLACK || piece->board->cells[down_idx[2]].color != BLACK) {
        return false;
      }
    }
    else if (piece->rot == 2) {
      for (int i = 0; i < 4; i++) {
        down_idx[i] = piece->board_index[i] + 10;
      }
      if (down_idx[2] > BOARD_MAX_IDX) {
        return false;
      }
      else if (piece->board->cells[down_idx[2]].color != BLACK || piece->board->cells[down_idx[3]].color != BLACK) {
        return false;
      }
    }
    else if (piece->rot == 3) {
      for (int i = 0; i < 4; i++) {
        down_idx[i] = piece->board_index[i] + 10;
      }
      if (down_idx[3] > BOARD_MAX_IDX) {
        return false;
      }
      else if (piece->board->cells[down_idx[0]].color != BLACK || piece->board->cells[down_idx[1]].color != BLACK || piece->board->cells[down_idx[3]].color != BLACK) {
        return false;
      }
    }
    else if (piece->rot == 4) {
      for (int i = 0; i < 4; i++) {
        down_idx[i] = piece->board_index[i] + 10;
      }
      if (down_idx[0] > BOARD_MAX_IDX) {
        return false;
      }
      else if (piece->board->cells[down_idx[0]].color != BLACK || piece->board->cells[down_idx[3]].color != BLACK) {
        return false;
      }
    }
    else {
      return false;
    }
  }
  //------------------- DEFAULT -------------------//
  else {
    return false;
  }

  for (int i = 0; i < 4; i++) piece->board->cells[piece->board_index[i]].color = BLACK;

  for (int i = 0; i < 4; i++) {
    piece->board_index[i] = down_idx[i];
    piece->board->cells[piece->board_index[i]].color = piece->color;
  }
  return true;
}

bool move_right(Piece *piece) {
  printf("move_right called\n");
  uint16_t right_idx[4];

  //----------------------------- T MOVE RIGHT -------------------------------------//
  if (piece->piece_shape == T) {
    if (piece->rot == 1) { // ROT = T
      for (int i = 0; i < 4; i++) {
        right_idx[i] = piece->board_index[i] + 1;
      }
      if (right_idx[2] % 10 == 0 || right_idx[2] % 10 == 1) {
        return false;
      }
      else if (piece->board->cells[right_idx[2]].color != BLACK || piece->board->cells[right_idx[3]].color != BLACK) {
        return false;
      }
    }
    else if (piece->rot == 2) { // ROT = -|
      for (int i = 0; i < 4; i++) {
        right_idx[i] = piece->board_index[i] + 1;
      }
      if (right_idx[1] % 10 == 0 || right_idx[1] % 10 == 1) {
        return false;
      }
      else if (piece->board->cells[right_idx[0]].color != BLACK || piece->board->cells[right_idx[1]].color != BLACK || piece->board->cells[right_idx[2]].color != BLACK) {
        return false;
      }
    }
    else if (piece->rot == 3) { // ROT = _|_
      for (int i = 0; i < 4; i++) {
        right_idx[i] = piece->board_index[i] + 1;
      }
      if (right_idx[0] % 10 == 0 || right_idx[0] % 10 == 1) {
        return false;
      }
      else if (piece->board->cells[right_idx[0]].color != BLACK || piece->board->cells[right_idx[3]].color != BLACK) {
        return false;
      }
    }
    else if (piece->rot == 4) { // ROT = |-
      for (int i = 0; i < 4; i++) {
        right_idx[i] = piece->board_index[i] + 1;
      }
      if (right_idx[0] > BOARD_MAX_IDX) {
        return false;
      }
      else if (piece->board->cells[right_idx[0]].color != BLACK || piece->board->cells[right_idx[2]].color != BLACK || piece->board->cells[right_idx[3]].color != BLACK) {
        return false;
      }
    }
    else {
      return false;
    }
  }

  //---------------------------------- I MOVE RIGHT -------------------------------//
  else if (piece->piece_shape == I) {
    if (piece->rot == 1) {
      for (int i = 0; i < 4; i++) {
        right_idx[i] = piece->board_index[i] + 1;
      }
      if (right_idx[3] % 10 == 0) {
        return false;
      }
      else if (piece->board->cells[right_idx[3]].color != BLACK) {
        return false;
      }
    }
    else if (piece->rot == 2) {
      for (int i = 0; i < 4; i++) {
        right_idx[i] = piece->board_index[i] + 1;
      }
      if (right_idx[1] % 10 == 0 || right_idx[1] % 10 == 1) {
        return false;
      }
      else if (piece->board->cells[right_idx[0]].color != BLACK || piece->board->cells[right_idx[1]].color != BLACK || piece->board->cells[right_idx[2]].color != BLACK || piece->board->cells[right_idx[3]].color != BLACK) {
        return false;
      }
    }
    else if (piece->rot == 3) {
      for (int i = 0; i < 4; i++) {
        right_idx[i] = piece->board_index[i] + 1;
      }
      if (right_idx[0] % 10 == 0 || right_idx[0] % 10 == 1) {
        return false;
      }
      else if (piece->board->cells[right_idx[0]].color != BLACK) {
        return false;
      }
    }
    else if (piece->rot == 4) {
      for (int i = 0; i < 4; i++) {
        right_idx[i] = piece->board_index[i] + 1;
      }
      if (right_idx[1] % 10 == 0 || right_idx[1] % 10 == 1) {
        return false;
      }
      else if (piece->board->cells[right_idx[0]].color != BLACK || piece->board->cells[right_idx[1]].color != BLACK || piece->board->cells[right_idx[2]].color != BLACK || piece->board->cells[right_idx[3]].color != BLACK) {
        return false;
      }
    }
    else {
      return false;
    }
  }
  //----------------------------- Z MOVE RIGHT -------------------------------------//
  else if (piece->piece_shape == Z) {
    if (piece->rot == 1 || piece->rot == 3) {
      for (int i = 0; i < 4; i++) {
        right_idx[i] = piece->board_index[i] + 1;
      }
      if (right_idx[3] % 10 == 0) {
        return false;
      }
      else if (piece->board->cells[right_idx[1]].color != BLACK || piece->board->cells[right_idx[3]].color != BLACK) {
        return false;
      }
    }
    else if (piece->rot == 2 || piece->rot == 4) { // ROT = -|
      for (int i = 0; i < 4; i++) {
        right_idx[i] = piece->board_index[i] + 1;
      }
      if (right_idx[1] % 10 == 0) {
        return false;
      }
      else if (piece->board->cells[right_idx[0]].color != BLACK || piece->board->cells[right_idx[1]].color != BLACK || piece->board->cells[right_idx[3]].color != BLACK) {
        return false;
      }
    }
    else {
      return false;
    }
  }

  //----------------------------- O MOVE RIGHT -------------------------------------//
  else if (piece->piece_shape == O) {
    if (piece->rot == 1 || piece->rot == 2 || piece->rot == 3 || piece->rot == 4) {
      for (int i = 0; i < 4; i++) {
        right_idx[i] = piece->board_index[i] + 1;
      }
      if (right_idx[3] % 10 == 0) {
        return false;
      }
      else if (piece->board->cells[right_idx[1]].color != BLACK || piece->board->cells[right_idx[3]].color != BLACK) {
        return false;
      }
    }
    else {
      return false;
    }
  }

  //---------------------------------- L MOVE RIGHT -------------------------------//
  else if (piece->piece_shape == L) {
    if (piece->rot == 1) {
      for (int i = 0; i < 4; i++) {
        right_idx[i] = piece->board_index[i] + 1;
      }
      if (right_idx[3] % 10 == 0) {
        return false;
      }
      else if (piece->board->cells[right_idx[2]].color != BLACK || piece->board->cells[right_idx[3]].color != BLACK) {
        return false;
      }
    }
    else if (piece->rot == 2) {
      for (int i = 0; i < 4; i++) {
        right_idx[i] = piece->board_index[i] + 1;
      }
      if (right_idx[3] % 10 == 0) {
        return false;
      }
      else if (piece->board->cells[right_idx[0]].color != BLACK || piece->board->cells[right_idx[1]].color != BLACK || piece->board->cells[right_idx[3]].color != BLACK) {
        return false;
      }
    }
    else if (piece->rot == 3) {
      for (int i = 0; i < 4; i++) {
        right_idx[i] = piece->board_index[i] + 1;
      }
      if (right_idx[0] % 10 == 0) {
        return false;
      }
      else if (piece->board->cells[right_idx[0]].color != BLACK || piece->board->cells[right_idx[3]].color != BLACK) {
        return false;
      }
    }
    else if (piece->rot == 4) {
      for (int i = 0; i < 4; i++) {
        right_idx[i] = piece->board_index[i] + 1;
      }
      if (right_idx[1] % 10 == 0) {
        return false;
      }
      else if (piece->board->cells[right_idx[0]].color != BLACK || piece->board->cells[right_idx[1]].color != BLACK || piece->board->cells[right_idx[2]].color != BLACK) {
        return false;
      }
    }
    else {
      return false;
    }
  }

  //----------------------- DEFAULT -----------------//
  else {
    return false;
  }

  for (int i = 0; i < 4; i++) piece->board->cells[piece->board_index[i]].color = BLACK;

  for (int i = 0; i < 4; i++) {
    piece->board_index[i] = right_idx[i];
    piece->board->cells[piece->board_index[i]].color = piece->color;
  }
  return true;
}

bool move_left(Piece *piece) {
  printf("move_left called\n");
  uint16_t left_idx[4];
  //------------------------------------- T MOVE LEFT -------------------------------//
  if (piece->piece_shape == T) {
    if (piece->rot == 1) { // ROT = T
      printf("T piece rot 1(T)\n");
      for (int i = 0; i < 4; i++) {
        left_idx[i] = piece->board_index[i] - 1;
        printf("left_idx[%d]=%d - board_idx[%d]=%d\n", i, left_idx[i], i, piece->board_index[i]);
      }
      if (left_idx[0] % 10 == 9 || left_idx[0] % 10 == 8) {
        return false;
      }
      else if (piece->board->cells[left_idx[0]].color != BLACK || piece->board->cells[left_idx[3]].color != BLACK) {
        return false;
      }
    }
    if (piece->rot == 2) { // ROT = -|
      for (int i = 0; i < 4; i++) {
        left_idx[i] = piece->board_index[i] - 1;
      }
      if (left_idx[3] % 10 == 9 || left_idx[3] % 10 == 8) {
        return false;
      }
      else if (piece->board->cells[left_idx[0]].color != BLACK || piece->board->cells[left_idx[3]].color != BLACK || piece->board->cells[left_idx[2]].color != BLACK) {
        return false;
      }
    }
    if (piece->rot == 3) { // ROT = _|_
      printf("T piece rot 3(_|_)\n");
      for (int i = 0; i < 4; i++) {
        left_idx[i] = piece->board_index[i] - 1;
        printf("left_idx[%d]=%d - board_idx[%d]=%d\n", i, left_idx[i], i, piece->board_index[i]);
      }
      if (left_idx[2] % 10 == 9 || left_idx[2] % 10 == 8) {
        return false;
      }
      else if (piece->board->cells[left_idx[2]].color != BLACK || piece->board->cells[left_idx[3]].color != BLACK) {
        return false;
      }
    }
    if (piece->rot == 4) { // ROT = |-
      for (int i = 0; i < 4; i++) {
        left_idx[i] = piece->board_index[i] - 1;
      }
      if (left_idx[1] % 10 == 9 || left_idx[1] % 10 == 8) {
        return false;
      }
      else if (piece->board->cells[left_idx[0]].color != BLACK || piece->board->cells[left_idx[1]].color != BLACK || piece->board->cells[left_idx[2]].color != BLACK) {
        return false;
      }
    }
  }

  //------------------------------------- I MOVE LEFT -------------------------------//

  else if (piece->piece_shape == I) {
    if (piece->rot == 1) {
      for (int i = 0; i < 4; i++) {
        left_idx[i] = piece->board_index[i] - 1;
      }
      if (left_idx[0] % 10 == 9) {
        return false;
      }
      else if (piece->board->cells[left_idx[0]].color != BLACK) {
        return false;
      }
    }
    if (piece->rot == 2) { // ROT = -|
      for (int i = 0; i < 4; i++) {
        left_idx[i] = piece->board_index[i] - 1;
      }
      if (left_idx[3] % 10 == 9) {
        return false;
      }
      else if (piece->board->cells[left_idx[0]].color != BLACK || piece->board->cells[left_idx[1]].color != BLACK || piece->board->cells[left_idx[3]].color != BLACK || piece->board->cells[left_idx[2]].color != BLACK) {
        return false;
      }
    }
    if (piece->rot == 3) { // ROT = _|_
      for (int i = 0; i < 4; i++) {
        left_idx[i] = piece->board_index[i] - 1;
      }
      if (left_idx[3] % 10 == 9) {
        return false;
      }
      else if (piece->board->cells[left_idx[3]].color != BLACK) {
        return false;
      }
    }
    if (piece->rot == 4) { // ROT = |-
      for (int i = 0; i < 4; i++) {
        left_idx[i] = piece->board_index[i] - 1;
      }
      if (left_idx[1] % 10 == 9) {
        return false;
      }
      else if (piece->board->cells[left_idx[0]].color != BLACK || piece->board->cells[left_idx[1]].color != BLACK || piece->board->cells[left_idx[2]].color != BLACK || piece->board->cells[left_idx[3]].color != BLACK) {
        return false;
      }
    }
  }

  //------------------------------------- Z MOVE LEFT -------------------------------//

  else if (piece->piece_shape == Z) {
    if (piece->rot == 1 || piece->rot == 3) { // ROT = T
      for (int i = 0; i < 4; i++) {
        left_idx[i] = piece->board_index[i] - 1;
      }
      if (left_idx[0] % 10 == 9) {
        return false;
      }
      else if (piece->board->cells[left_idx[0]].color != BLACK || piece->board->cells[left_idx[2]].color != BLACK) {
        return false;
      }
    }
    else if (piece->rot == 2 || piece->rot == 4) { // ROT = -|
      for (int i = 0; i < 4; i++) {
        left_idx[i] = piece->board_index[i] - 1;
      }
      if (left_idx[2] % 10 == 9) {
        return false;
      }
      else if (piece->board->cells[left_idx[0]].color != BLACK || piece->board->cells[left_idx[2]].color != BLACK || piece->board->cells[left_idx[3]].color != BLACK) {
        return false;
      }
    }
  }

  //----------------------------- O MOVE RIGHT -------------------------------------//
  else if (piece->piece_shape == O) {
    if (piece->rot == 1 || piece->rot == 2 || piece->rot == 3 || piece->rot == 4) {
      for (int i = 0; i < 4; i++) {
        left_idx[i] = piece->board_index[i] - 1;
      }
      if (left_idx[0] % 10 == 9) {
        return false;
      }
      else if (piece->board->cells[left_idx[0]].color != BLACK || piece->board->cells[left_idx[2]].color != BLACK) {
        return false;
      }
    }
    else {
      return false;
    }
  }
  //----------------------------- L MOVE RIGHT -------------------------------------//
  else if (piece->piece_shape == L) {
    if (piece->rot == 1) {
      for (int i = 0; i < 4; i++) {
        left_idx[i] = piece->board_index[i] - 1;
      }
      if (left_idx[0] % 10 == 9) {
        return false;
      }
      else if (piece->board->cells[left_idx[0]].color != BLACK || piece->board->cells[left_idx[3]].color != BLACK) {
        return false;
      }
    }
    if (piece->rot == 2) { // ROT = -|
      for (int i = 0; i < 4; i++) {
        left_idx[i] = piece->board_index[i] - 1;
      }
      if (left_idx[1] % 10 == 9) {
        return false;
      }
      else if (piece->board->cells[left_idx[0]].color != BLACK || piece->board->cells[left_idx[1]].color != BLACK || piece->board->cells[left_idx[2]].color != BLACK) {
        return false;
      }
    }
    if (piece->rot == 3) { // ROT = _|_
      for (int i = 0; i < 4; i++) {
        left_idx[i] = piece->board_index[i] - 1;
      }
      if (left_idx[3] % 10 == 9) {
        return false;
      }
      else if (piece->board->cells[left_idx[3]].color != BLACK || piece->board->cells[left_idx[2]].color) {
        return false;
      }
    }
    if (piece->rot == 4) { // ROT = |-
      for (int i = 0; i < 4; i++) {
        left_idx[i] = piece->board_index[i] - 1;
      }
      if (left_idx[3] % 10 == 9) {
        return false;
      }
      else if (piece->board->cells[left_idx[0]].color != BLACK || piece->board->cells[left_idx[1]].color != BLACK || piece->board->cells[left_idx[3]].color != BLACK) {
        return false;
      }
    }
  }

  //------------------------------------- DEFAULT -------------------------------//

  else {
    return false;
  }

  for (int i = 0; i < 4; i++) piece->board->cells[piece->board_index[i]].color = BLACK;

  for (int i = 0; i < 4; i++) {
    piece->board_index[i] = left_idx[i];
    piece->board->cells[piece->board_index[i]].color = piece->color;
  }
  return true;
}
