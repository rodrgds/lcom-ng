#include "player.h"

Player* newPlayer(char* name, playerBall ballType, bool isPlaying){
  Player* player = (Player*)malloc(sizeof(Player));
  player->name = name;
  player->ballType = ballType;
  player->isPlaying = isPlaying;
  return player;
}

void destroyPlayer(Player* player){
  free(player);
}

int drawInGamePlayerName(Player* player, xpm_image_t* font, int x, int y, int spacing){
  if(drawText(player->name,font,x,y,spacing)) return 1;
  return 0;
}
