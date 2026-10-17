#include "player.h"

Player *(createPlayer)() {
  Player *player = (Player *) malloc(sizeof(Player));
  player->pos.x = 575;
  player->pos.y = 431;
  player->score = 0;
  player->canShoot = true;

  return player;
}

int16_t (getPlayerX)(Player *player) {
  return player->pos.x;
}

void (setPlayerX)(Player *player, uint16_t x) {
  player->pos.x = x;
}

int16_t (getPlayerY)(Player *player) {
  return player->pos.y;
}

void (setPlayerY)(Player *player, uint16_t y) {
  player->pos.y = y;
}

uint16_t (getPlayerScore)(Player *player) {
  return player->score;
}

void (setPlayerScore)(Player *player, uint16_t score) {
  player->score = score;
}

bool (getPlayerCanShoot)(Player *player) {
  return player->canShoot;
}

void (setPlayerCanShoot)(Player *player, bool canShoot) {
  player->canShoot = canShoot;
}

void (destroyPlayer)(Player *player) {
  free(player);
}
