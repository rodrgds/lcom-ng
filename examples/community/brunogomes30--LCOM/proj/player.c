#include "player.h"
#include "bonecoparado.h"
#include "cursor.h"
#include "images.h"
#include "linkedEntities.h"
#include "math.h"
#include "pistoladeagua.h"

void initPlayer(Game *game, Player *player) {
  player->sprite = createSprite(&player_image, 300, 300);
  player->sprite->isMovementUnique = 1;
  player->input = 0x00000000;
  player->cursor = createSprite(&cursor_image, 400, 400);
  player->cursor->isMovementUnique = 1;
  player->gun = getPistol(game, 1, 0, 0);
  player->defaultImage = &player_image;
  player->movingImages[0] = &player_image_left;
  player->movingImages[1] = &player_image_right;
  player->movingImageIndex = 0;
  player->currentMovingTick = 0;
  player->score = 0;
}

void updatePlayerAnimation(Player *player) {
  if (player->speedX != 0 || player->speedY != 0) {
    if (++player->currentMovingTick == 10) {
      player->movingImageIndex = (player->movingImageIndex + 1) % 2;
      player->sprite->image = player->movingImages[player->movingImageIndex];
      player->currentMovingTick = 0;
    }
  }
  else {
    player->sprite->image = player->defaultImage;
    player->movingImageIndex = 0;
    player->currentMovingTick = 0;
  }
}

void player_kb_input(Player *player, uint32_t kbd_code) {
  switch (kbd_code & ~BIT(7)) {
    case KEYCODE_W:
      player->input = (player->input & ~BIT(0)) | !is_break_code(kbd_code);
      break;
    case KEYCODE_A:
      player->input = (player->input & ~BIT(1)) | !is_break_code(kbd_code) << 1;
      break;
    case KEYCODE_S:
      player->input = (player->input & ~BIT(2)) | !is_break_code(kbd_code) << 2;
      break;
    case KEYCODE_D:
      player->input = (player->input & ~BIT(3)) | !is_break_code(kbd_code) << 3;
      break;
  }
}

void player_mouse_input(Game *game, struct packet dataPacket) {
  Player * player = game->players[0];
  player->cursor->x += dataPacket.delta_x;
  player->cursor->y -= dataPacket.delta_y;
  keepInsideScreen(game->x_res, game->y_res, player->cursor);



  if (dataPacket.lb) {
    player->input = (player->input | BIT(4));
  } else {
    player->input = (player->input & ~BIT(4));
  }
}

void player_movement(Game *game, double gameSpeed) {
  Player * player = game->players[0];
  unsigned timeSpent = gameSpeed * 17; // 1000 is aprox 1 sec (in 60FPS)
  //Y axis
  double speed = 3.0;
  if (INPUT_W(player->input) || INPUT_S(player->input)) {
    if (INPUT_W(player->input)) {
      player->speedY = -speed;
    }
    if (INPUT_S(player->input)) {
      player->speedY = speed;
    }
  }
  else {
    //reset y speed
    player->speedY = 0;
  }

  //X axis
  if (INPUT_A(player->input) || INPUT_D(player->input)) {
    if (INPUT_A(player->input)) {
      player->speedX = -speed;
    }
    if (INPUT_D(player->input)) {
      player->speedX = speed;
    }
  }
  else {
    player->speedX = 0;
  }

  player->sprite->realx += player->speedX;
  player->sprite->realy += player->speedY;
  player->sprite->x = (int) player->sprite->realx;
  player->sprite->y = (int) player->sprite->realy;

  int gun_pos_x, gun_pos_y;
  getRotationCoordinate(player->sprite, player->sprite->x + PLAYER_GUN_POSITION_X, player->sprite->y + PLAYER_GUN_POSITION_Y, &gun_pos_x, &gun_pos_y);
  player->gun->sprite->x = gun_pos_x;
  player->gun->sprite->y = gun_pos_y;
  player->gun->sprite->realx = (double) gun_pos_x;
  player->gun->sprite->realy = (double) gun_pos_y;
  if (player->gun->timeToNextShot >= timeSpent) {
    player->gun->timeToNextShot -= timeSpent;
  }
  else {
    player->gun->timeToNextShot = 0;
  }

  if (INPUT_LB(player->input) && (player->gun->timeToNextShot == 0)) {
    shoot(player->gun, player->cursor->x, player->cursor->y);
    player->input = player->input & ~BIT(4);
  }
  keepInsideScreen(game->x_res, game->y_res, player->sprite);
}

void updateRotation(Player *player) {
  player->sprite->rotation = getRotation(player->sprite, player->cursor->x, player->cursor->y);
  player->gun->sprite->rotation = player->sprite->rotation;
}
