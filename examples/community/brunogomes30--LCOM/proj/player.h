#ifndef PLAYER_H
#define PLAYER_H
#include <lcom/lcf.h>
#include "i8042.h"
#include "kbd.h"
#include "graphics.h"
#include "sprite.h"
#include "gun.h"
#include "game.h"

/** @defgroup player 
 * @{
 *
 * Functions related to player 
 */

static xpm_image_t bullet_image;

#define INPUT_W(input) (input & BIT(0))
#define INPUT_A(input) (input & BIT(1))
#define INPUT_S(input) (input & BIT(2))
#define INPUT_D(input) (input & BIT(3))
#define INPUT_LB(input) (input & BIT(4))

#define PLAYER_GUN_POSITION_X 28
#define PLAYER_GUN_POSITION_Y 10

struct Player {
  unsigned points;
  int hp;

  Sprite * sprite;
  int speedX, speedY;
  uint8_t input;
  Sprite * cursor;
  //xpm_image_t moving_animation[2] = (player_image, player_image_left, player_image_right);
  Gun *gun;
  int score;
  //moving animation
  xpm_image_t *movingImages[2];
  xpm_image_t *defaultImage;
  uint8_t movingImageIndex;
  uint8_t currentMovingTick;
};




/**
 * @brief Init player in the game
 * 
 * @param game 
 * @param player 
 */
void initPlayer(Game *game, Player *player);

/**
 * @brief Handles keyboard input
 * 
 * @param player 
 * @param kbd_code 
 */
void player_kb_input(Player *player, uint32_t kbd_code);

/**
 * @brief Handles mouse input
 * 
 * @param game 
 * @param dataPacket 
 */
void player_mouse_input(Game * game, struct packet dataPacket);

/**
 * @brief Moves player
 * 
 * @param game 
 * @param gameSpeed 
 */
void player_movement(Game *game, double gameSpeed) ;

/**
 * @brief Updates Player's animation
 * 
 * @param player 
 */
void updatePlayerAnimation(Player *player);

/**
 * @brief Sets player facing the cursor
 * 
 * @param player 
 */
void updateRotation(Player *player);

#endif
