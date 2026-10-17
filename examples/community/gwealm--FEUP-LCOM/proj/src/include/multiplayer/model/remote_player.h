#pragma once

#include "game/model/cell_background.h"
#include "multiplayer/layout.h"

typedef struct {
  size_t pos;
  cell_background background[COLS * ROWS];
} remote_player;

void (remote_player_reset)();

remote_player *(remote_player_get)();
