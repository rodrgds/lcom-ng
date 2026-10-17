/**
 * @file fire.h
 * @brief Module for handling player fire shots in the game.
 */

#ifndef _FIRE_H_
#define _FIRE_H_

#include <lcom/lcf.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include "sprites/projPixmap.h"
#include "sprites/sprite.h"
#include "devices/videocard.h"
#include "enemy.h"
#include "playerShip.h"
#include "cursor.h"

#define MAX_FIRE 100 /**< Max quantity of enemy fire shots available per frame */
#define FIRE_RELOAD 60 /**< Waiting time of 1 second after reaching the 100 fire shots */

/** @defgroup fire Fire
 * @{
 *
 * @brief  Fire related functions
 * 
 */

/**
 * @brief Struc representing a player fire shot in the game.
 */
typedef struct {
  uint16_t x; /**< X-coordinate of the player fire shot */
  uint16_t y; /**< Y-coordinate of the player fire shot */
  uint16_t v_x; /**< X-velocity of the player fire shot */
  uint16_t v_y; /**< Y-velocity of the player fire shot */
  bool alive; /**< fire shot alive status */
  Sprite * curr_sprite; /**< Current sprite of the player fire projectile */

} Fire;


/**
 * @brief Creates a new player fire projectile.
 * 
 * @param sprite XPM map of the sprite for the player fire shot.
 * @param x Initial X-coordinate of the player fire shot.
 * @param y Initial Y-coordinate of the player fire shot.
 * @param speed Speed of the player fire shot.
 * @return Pointer to the created player fire shot.
 */
Fire* createFire(xpm_map_t sprite, uint16_t x, uint16_t y, uint16_t speed);

/**
 * @brief Draws the player fire shot on the screen.
 * 
 * @param fire Pointer to the player fire shot to be drawn.
 */
void drawFire(Fire* fire);

/**
 * @brief Creates player fire shots.
 * 
 * @param playerShip Pointer to the player ship creating the fire shot.
 */
void createSpaceShipFire(PlayerShip* playerShip);

/**
 * @brief Updates the player fire shots.
 */
void moveFire();

/**
 * @brief Checks if an enemy has been hit by the player fire shots.
 * 
 * @param enemy Pointer to the enemy being checked for collision.
 * @param fire Pointer to the player fire shot.
 * @return 1 if enemy is hit, 0 otherwise.
 */
int checkEnemyHit(Enemy* enemy, Fire* fire );

/**
 * @brief Destroys the player fire shot and frees its memory space.
 * 
 * @param enemyFire Pointer to the player fire shot to be destroyed.
 */
void destroyFire(Fire* fire);

/** @} end of fire */

#endif /* _FIRE_H_ */


