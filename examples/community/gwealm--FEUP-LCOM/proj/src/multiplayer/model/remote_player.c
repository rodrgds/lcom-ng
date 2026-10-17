#include "multiplayer/model/remote_player.h"

static remote_player r;

void (remote_player_reset)() {
  memset(&r, 0, sizeof(remote_player));
}

remote_player *(remote_player_get)() {
  return &r;
}
