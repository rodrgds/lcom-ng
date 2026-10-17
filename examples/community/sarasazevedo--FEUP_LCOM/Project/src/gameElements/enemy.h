/**
 * @file enemy.h
 * @brief Module for handling enemies in the game.
 */

#ifndef _LCOM_ENEMY_H_
#define _LCOM_ENEMY_H_


#include <lcom/lcf.h>
#include <stdint.h>
#include "sprites/projPixmap.h"
#include "sprites/sprite.h"
#include "enemyExplosion.h"

#define MAX_MOVEMENT 190 /**< Maximum movement displacement for enemies*/
#define ENEMY_FIRE_RATE 5 /**< Enemy fire rate*/
#define ENEMY_POINTS 5 /**< Points awarded to player for destroying an enemy*/
#define ENEMY_SPEED 1 /**<Enemy movement speed*/

/** @defgroup enemy Enemy
 * @{
 *
 * @brief  Enemy related functions
 * 
 */

/**
 * @enum Direction
 * @brief Enum for the direction of the enemy.
*/

typedef enum{
  LEFT, /**< Moving left */
  RIGHT, /**< Moving right */
} Direction;

/**
 * @struct Enemy
 * @brief Struct representing an enemy.
*/

typedef struct {
  uint16_t x; /**< X position of the enemy */
  uint16_t y; /**< Y position of the enemy */
  bool alive; /**< Enemy alive status */
  Sprite** sprites; /**< Array of enemy sprites */
  Sprite * curr_sprite;  /**< Current sprite of the enemy */
  uint32_t sprite_quant;  /**< Quantity of sprites */
  uint32_t curr_sprite_no; /**< Current sprite number */
  Direction direction; /**< Direction of the enemy */
} Enemy;


/**
 * @brief Creates a new enemy.
 * @param sprite1 First sprite of the enemy
 * @param sprite2 Second sprite of the enemy
 * @param x Initial X position of the enemy
 * @param y Initial Y position of the enemy
 * @return Pointer to the created enemy
 */
Enemy* createEnemy(xpm_map_t sprite1, xpm_map_t sprite2, uint16_t x, uint16_t y);

/**
 * @brief Draws the current sprite of the enemy.
 * @param enemy Pointer to the enemy
 */
void drawCurrEnemySprite(Enemy* sprite);

/**
 * @brief Destroys the enemy and frees its memory space.
 * @param enemy Pointer to the enemy to be destroyed
 */
void destroyEnemy(Enemy* enemy);

/**
 * @brief Sets the initial positions of all enemies.
 */
void setEnemiesInitialPos();

/**
 * @brief Updates the enemies and their explosions.
 */
void updateEnemiesAndExplosions();

/** @} end of enemy */

#endif /* _LCOM_ENEMY_H_ */


