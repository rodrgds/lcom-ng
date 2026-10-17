#include "game/model/game.h"

static game g;

game *(game_get)() {
  return &g;
}

void (game_reset)() {
  memset(&g, 0, sizeof(game));
}
