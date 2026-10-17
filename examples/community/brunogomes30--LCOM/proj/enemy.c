#include "enemy.h"
#include "images.h"

xpm_image_t enemy1_image;
xpm_image_t enemy2_image;

//static xpm_image_t *explosion_images[4] = {&explosion_0_image, &explosion_1_image, &explosion_2_image, &explosion_3_image};

Enemy createEnemy(Game *game, Player *player, int x, int y) {
  Enemy enemy;
  xpm_image_t *image;
  if (rand() % 2) {
    image = &enemy1_image;
  }
  else {
    image = &enemy2_image;
  }
  enemy.sprite = createSprite(image, x, y);
  if (rand() % 2) {
    //Enemy starts with a pistol
    enemy.gun = getPistol(game, 1, x, y);
    enemy.score = 100;
  }
  else {
    //Enemy starts with a SMG
    enemy.gun = getSMG(game, 1, x, y);
    enemy.score = 150;
  }
  enemy.hp = 100;

  //Set enemy moving towards player
  setEnemySpeed(&enemy, player);
  shoot(enemy.gun, player->sprite->x, player->sprite->y);
  return enemy;
}

void setEnemySpeed(Enemy *enemy, Player *player) {
  setSpeed(enemy->sprite, 2, player->sprite->x - enemy->sprite->x, player->sprite->y - enemy->sprite->y);
  enemy->gun->sprite->speedX = enemy->sprite->speedX;
  enemy->gun->sprite->speedY = enemy->sprite->speedY;
  enemy->sprite->rotation = enemy->sprite->rotation;
}

void enemyAction(Enemy *enemy, Player *player) {
  setEnemySpeed(enemy, player);
  if (enemy->gun->timeToNextShot == 0) {
    shoot(enemy->gun, player->sprite->x, player->sprite->y);
  }
}

void moveEnemy(Enemy *enemy, double gameSpeed) {
  int timeSpent = gameSpeed * 17;
  if (enemy->gun->timeToNextShot >= timeSpent) {
    enemy->gun->timeToNextShot -= timeSpent;
  }
  else {
    enemy->gun->timeToNextShot = 0;
  }

  moveSprite(gameSpeed, enemy->sprite);
  int gun_pos_x, gun_pos_y;
  getRotationCoordinate(enemy->sprite, enemy->sprite->x + PLAYER_GUN_POSITION_X, enemy->sprite->y + PLAYER_GUN_POSITION_Y, &gun_pos_x, &gun_pos_y);
  enemy->gun->sprite->x = gun_pos_x;
  enemy->gun->sprite->y = gun_pos_y;
  enemy->gun->sprite->realx = (double) gun_pos_x;
  enemy->gun->sprite->realy = (double) gun_pos_y;
  enemy->gun->sprite->rotation = enemy->sprite->rotation;
}
