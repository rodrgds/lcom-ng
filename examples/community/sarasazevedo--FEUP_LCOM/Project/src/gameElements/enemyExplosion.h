/**
 * @file enemyExplosion.h
 * @brief Module for handling enemy explosions in the game.
 */

#ifndef _LCOM_ENEMYEXPLOSION_H_
#define _LCOM_ENEMYEXPLOSION_H_


#include <lcom/lcf.h>
#include <stdint.h>
#include "sprites/projPixmap.h"
#include "sprites/sprite.h"

/** @defgroup enemyExplosion EnemyExplosion
 * @{
 *
 * @brief  EnemyExplosion related functions
 * 
 */


/**
 * @brief Struct representing an enemy explosion in the game.
 */

typedef struct {
  uint16_t x; /**< X-coordinate of the enemy explosion */
  uint16_t y; /**< Y-coordinate of the enemy explosion */
  bool alive; /**< Explosion alive status */
  Sprite** sprites; /**< Array of sprites representing the explosion animation */
  Sprite * curr_sprite; /**< Current sprite of the explosion */
  uint32_t sprite_quant; /**< Total number of sprites in the animation */
  uint32_t curr_sprite_no; /**< Current sprite number */
} EnemyExplosion;

/**
 * @brief Creates a new enemy explosion.
 * 
 * @param sprite1 XPM map of the first sprite for the explosion animation.
 * @param sprite2 XPM map of the second sprite for the explosion animation.
 * @param sprite3 XPM map of the third sprite for the explosion animation.
 * @param sprite4 XPM map of the fourth sprite for the explosion animation.
 * @param sprite5 XPM map of the fifth sprite for the explosion animation.
 * @param x Initial X-coordinate of the enemy explosion.
 * @param y Initial Y-coordinate of the enemy explosion.
  * @return Pointer to the created enemy explosion.
 */
EnemyExplosion* createEnemyExplosion(xpm_map_t sprite1, xpm_map_t sprite2, xpm_map_t sprite3, xpm_map_t sprite4, xpm_map_t sprite5, uint16_t x, uint16_t y);

/**
 * @brief Updates the sprite of the enemy explosion.
 * 
 * @param explosion Pointer to the enemy explosion.
 */
void  updateEnemyExplosionSprite(EnemyExplosion* sprite);

/**
 * @brief Draws the enemy explosion on the screen.
 * 
 * @param explosion Pointer to the enemy explosion.
 */
void drawEnemyExplosion(EnemyExplosion* explosion);

/**
 * @brief Destroys the enemy explosion and frees its memory space.
 * 
 * @param explosion Pointer to the enemy explosion to be destroyed.
 */
void destroyEnemyExplosion(EnemyExplosion* explosion);

/** @} end of enemyExplosion */

#endif /* _LCOM_ENEMYEXPLOSION_H_ */
