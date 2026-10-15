

#include "PlayerInfo.h"
#include <stdlib.h>

PlayerInfo* newPlayerInfo() {
  PlayerInfo* playerInfo = (PlayerInfo*) malloc(sizeof(PlayerInfo));
  playerInfo->isPetting = false;

  return playerInfo;
}
void deletePlayerInfo(PlayerInfo* playerInfo) {
  free(playerInfo);
}

