/**
 * @file enemyFire.h
 * @brief Module for handling enemy fire  in the game.
 */

#ifndef _ENEMYFIRE_H_
#define _ENEMYFIRE_H_

#include <lcom/lcf.h>
#include <stdint.h>
#include <stdbool.h>
#include "sprites/projPixmap.h"
#include "sprites/sprite.h"
#include "devices/videocard.h"
#include "enemy.h"
#include "playerShip.h"
#include "game.h"


#define MAX_ENEMY_FIRE 60 /**< Max quantity of enemy fire shots available per frame */

/** @defgroup enemyFire EnemyFire
 * @{
 *
 * @brief  EnemyFire related functions
 * 
 */

/**
 * @brief Structure representing an enemy fire shot in the game.
 */

typedef struct {
  uint16_t x; /**< X-coordinate of the enemy fire projectile */
  uint16_t y; /**< Y-coordinate of the enemy fire projectile */
  uint16_t v_x; /**< X-velocity of the enemy fire projectile */
  uint16_t v_y; /**< Y-velocity of the enemy fire projectile */
  bool alive; /**< enemy fire shot alive status */
  Sprite * curr_sprite; /**< Current sprite of the enemy fire projectile */

} EnemyFire;


/**
 * @brief Creates a new enemy fire projectile.
 * 
 * @param sprite XPM map of the sprite for the enemy fire shot.
 * @param x Initial X-coordinate of the enemy fire shot.
 * @param y Initial Y-coordinate of the enemy fire shot.
 * @param speed Speed of the enemy fire shot.
 * @return Pointer to the created enemy fire shot.
 */

EnemyFire* createEnemyFire(xpm_map_t sprite, uint16_t x, uint16_t y, uint16_t speed);

/**
 * @brief Draws the enemy fire shot on the screen.
 * 
 * @param enemyFire Pointer to the enemy fire shot to be drawn.
 */
void drawEnemyFire(EnemyFire* enemyFire);

/**
 * @brief Creates enemy fire shots.
 * 
 * @param enemy Pointer to the enemy creating the fire shot.
 */
void createEnemiesFire(Enemy* enemy);

/**
 * @brief Updates the enemy fire shot position.
 */
void moveEnemyFire();

/**
 * @brief Checks if the player ship has been hit by an enemy fire shot.
 * 
 * @param playerShip Pointer to the player spaceship.
 * @param enemyFire Pointer to the enemy fire shot.
 */
void checkSpaceShipHit(PlayerShip* playerShip, EnemyFire* enemyFire );

/**
 * @brief Destroys the enemy fire shot and frees its memory space.
 * 
 * @param enemyFire Pointer to the enemy fire shot to be destroyed.
 */
void destroyEnemyFire(EnemyFire* enemyFire);

/** @} end of enemyFire */

#endif /* _ENEMYFIRE_H_ */


