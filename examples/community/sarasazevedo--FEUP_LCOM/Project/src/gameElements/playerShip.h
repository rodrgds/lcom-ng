/**
 * @file playerShip.h
 * 
 *  @brief Module for handling player spaceship in the game.
 */

#ifndef _PLAYERSHIP_H_
#define _PLAYERSHIP_H_

#include <lcom/lcf.h>
#include <stdint.h>
#include <stdbool.h>
#include "sprites/projPixmap.h"
#include "sprites/sprite.h"
#include "gameElements/spaceship_explosion.h"
#include "gameElements/enemy.h"

#define SPACESHIP_SPEED 3 /**< Speed of the spaceship */


/** @defgroup playership PlayerShip
 * @{
 *
 * @brief  PlayerShip related functions
 * 
 */

/**
 * @brief Struct representing the player spaceship.
*/

typedef struct {
  uint16_t x; /**< X-coordinate of the spaceship */
  uint16_t y; /**< Y-coordinate of the spaceship */
  bool alive; /**< Spaceship alive status */

  int currLives; /**< Current number of lives of the spaceship */
  int currScore; /**< Current score of the spaceship */
  
  Sprite * curr_sprite; /**< Current sprite of the spaceship */
  Spaceship_Explosion* spaceshipExplosion; /**< Spaceship explosion object */
} PlayerShip;

/**
 * @brief Creates a player spaceship object.
 * 
 * @param sprite The sprite of the spaceship.
 * @param x The initial x-coordinate of the spaceship.
 * @param y The initial y-coordinate of the spaceship.
 * @return A pointer to the created player spaceship.
 */
PlayerShip* createPlayerShip(xpm_map_t sprite, uint16_t x, uint16_t y);

/**
 * @brief Draws the player spaceship on the screen.
 * 
 * @param playerShip A pointer to the player spaceship.
 */
void drawPlayerShip(PlayerShip* playerShip);

/**
 * @brief Creates a spaceship explosion effect.
 * 
 * @param playerShip A pointer to the player spaceship.
 */
void makeSpaceShipExplosion(PlayerShip* playerShip);

/**
 * @brief Updates the spaceship explosion effect.
 */
void updateSpaceshipExplosions();

/**
 * @brief Destroys the player spaceship and frees its memory space.
 * 
 * @param playerShip A pointer to the player spaceship object.
 */
void destroyPlayerShip(PlayerShip* playerShip);

/**
 * @brief Updates the player's score.
 * 
 * @param playership A pointer to the player spaceship.
 */
void updatePlayerScore(PlayerShip* playership);

/** @} end of playerShip */

#endif /* _PLAYERSHIP_H_ */



