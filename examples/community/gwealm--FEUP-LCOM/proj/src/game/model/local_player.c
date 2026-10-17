#include "game/model/local_player.h"

static local_player p;

local_player *(local_player_get)() {
  return &p;
}

void (local_player_reset)() {
  memset(&p, 0, sizeof(local_player));
}

char (local_player_read)() {
  return p.board[p.pos];
}

char (local_player_read_before)() {
  return p.board[p.pos - 1];
}

char (local_player_read_after)() {
  return p.board[p.pos + 1];
}

char (local_player_read_last_of_row)() {
  unsigned long pos = (p.y + 1) * COLS - 1;
  return p.board[pos];
}

void (local_player_write)(char ch) {
  p.board[p.pos] = ch;
}

void (local_player_goto)(uint8_t x, uint8_t y) {
  p.x = x;
  p.y = y;

  p.pos = y * COLS + x;
}

void (local_player_forward)() {
  p.x++;
  p.pos++;
}

void (local_player_backward)() {
  p.x--;
  p.pos--;
}

void (local_player_downward)() {
  p.pos += COLS - p.x;

  p.x = 0;
  p.y++;
}
