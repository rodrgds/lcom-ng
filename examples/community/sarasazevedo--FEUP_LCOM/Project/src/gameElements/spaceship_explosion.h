/**
 * @file spaceship_explosion.h
 * @brief Module file for handling player spaceship explosions in the game.
 */

#ifndef _LCOM_SPACESHIP_EXPLOSION_H_
#define _LCOM_SPACESHIP_EXPLOSION_H_


#include <lcom/lcf.h>
#include <stdint.h>
#include "sprites/projPixmap.h"
#include "sprites/sprite.h"


/** @defgroup spaceship_explosion Spaceship_Explosion
 * @{
 *
 * @brief  Spaceship_Explosion related functions
 * 
 */


/**
 * @brief Structure representing a player spaceship explosion.
 */
typedef struct {
  uint16_t x; /**< X-coordinate of the explosion */
  uint16_t y; /**< Y-coordinate of the explosion */
  bool alive; /**< Explosion alive status */
  Sprite** sprites; /**< Array of explosion sprites */
  Sprite * curr_sprite; /**< Current explosion sprite */
  uint32_t sprite_quant; /**< Number of explosion sprites */
  uint32_t curr_sprite_no; /**< Index of the current explosion sprite */
} Spaceship_Explosion; 

/**
 * @brief Creates a spaceship explosion object.
 * 
 * @param sprite1 The first sprite of the explosion.
 * @param sprite2 The second sprite of the explosion.
 * @param sprite3 The third sprite of the explosion.
 * @param sprite4 The fourth sprite of the explosion.
 * @param x The initial x-coordinate of the explosion.
 * @param y The initial y-coordinate of the explosion.
 * @return A pointer to the created player spaceship explosion.
 */
Spaceship_Explosion* createExplosion(xpm_map_t sprite1, xpm_map_t sprite2, xpm_map_t sprite3, xpm_map_t sprite4, uint16_t x, uint16_t y);

/**
 * @brief Updates the current sprite of the spaceship explosion.
 * 
 * @param explosion A pointer to the spaceship explosion.
 */
void updateSpaceshipExplosionSprite(Spaceship_Explosion* explosion);

/**
 * @brief Draws the current sprite of the spaceship explosion.
 * 
 * @param explosion A pointer to the spaceship explosion.
 */
void drawCurrExplosionSprite(Spaceship_Explosion* sprite);

/**
 * @brief Destroys the spaceship explosion  and frees its memory space.
 * 
 * @param explosion A pointer to the spaceship explosion.
 */
void destroySpaceshipExplosion(Spaceship_Explosion* explosion);

/** @} end of spaceship_explosion */

#endif /* _LCOM_SPACESHIP_EXPLOSION_H_ */
 




