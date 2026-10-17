#ifndef GUN_H
#define GUN_H
#include "game.h"
#include "sprite.h"
#include "images.h"
#include "linkedEntities.h"

/** @defgroup gun 
 * @{
 *
 * Functions related to gun 
 */

struct Gun{
    Game *game;
    Sprite *sprite;
    int shotSpeed;
    uint16_t shotDelay; //Time between shots
    uint16_t timeToNextShot; // Time remaining for the next available shot
    int8_t isEquipped, isShooting, current_image, current_animation_tick;
    xpm_image_t *images[4];

    
};

struct Bullet{
    Gun *origin;
    Sprite *sprite;
};

/**
 * @brief Create a gun with normal firerate
 * 
 * @param game 
 * @param isEquipped 
 * @param x 
 * @param y 
 * @return Gun* 
 */
Gun *getPistol(Game *game, uint8_t isEquipped, int x, int y);

/**
 * @brief Creates a gun with increase firerate
 * 
 * @param game 
 * @param isEquipped 
 * @param x 
 * @param y 
 * @return Gun* 
 */
Gun *getSMG(Game *game, uint8_t isEquipped, int x, int y);

/**
 * @brief Makes the gun shoot at the target
 * 
 * @param gun 
 * @param targetX 
 * @param targetY 
 */
void shoot(Gun *gun, unsigned targetX,unsigned targetY);

/**
 * @brief Updates gun animation, checks if its shooting
 * 
 * @param gun 
 */
void updateGunAnimation(Gun * gun);

#endif
