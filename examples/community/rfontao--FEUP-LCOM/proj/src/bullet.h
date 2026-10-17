#ifndef _BULLET_H_
#define _BULLET_H_


#include <lcom/lcf.h>
#include <lcom/liblm.h>
#include <lcom/proj.h>

#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>

#include "sprite.h"
#include "collisions.h"

#include "resources/bullet_xpm.xpm"

/** @defgroup Bullet Bullet
 * @{
 *
 * Constants and functions of the bullets
 */


#define MAX_BULLET_COUNT 200 /**< @brief Maximum amount of bullets that can exist at the same time */

#define BULLET_VELOCITY 8 /**< @brief Velocity of each bullet */


/**
 * @brief Bullet Struct
 * 
 */
typedef struct{
        Sprite * sprite;                /**< @brief Bullet sprite */
        unsigned short int id;          /**< @brief ID of the bullet */
        unsigned short int group;       /**< @brief Group of the bullet(Ally or Enemy) as declared                                       in collision.h*/
} Bullet;

/**
 * @brief Create a new Bullet object
 * 
 * @param initial_x     Initial X pos of the bullet
 * @param initial_y     Initial Y pos of the bullet
 * @param initial_vx    Initial X velocity of the bullet
 * @param initial_vy    Initial Y velocity of the bullet
 * @param group         Group of the bullet
 * @return Bullet*      The new bullet
 */
Bullet * createBullet(int initial_x, int initial_y, int initial_vx,int initial_vy, unsigned short int group);


/**
 * @brief Creates a new bullet (making the velocity unitary) and adds it to the bullets array in   *        the first available spot
 * 
 * @param bullets       The bullet array in which it will be placed
 * @param xi            X pos of the bullet source
 * @param yi            Y pos of the bullet source
 * @param xf            X pos of the bullet destination
 * @param yf            Y pos of the bullet destination
 * @param group         Group of the bullet
 */
void shoot(Bullet * bullets[], int xi,int yi, int xf,int yf, unsigned short int group);

/**
 * @brief Draws all the bullets on the screen and the collidibles based on the source(Ally or Enemy)
 * 
 * @param bullets       The bullet array
 * @param car           The Car object
 * @param object        The Objective object
 */
void drawBullets(Bullet * bullets[], Car* car, Objective* object);

/**
 * @brief Removes the bullet that are out of the bounds of the game
 * 
 * @param bullets       The bullet array
 */
void removeBullets(Bullet * bullets[]);

/**
 * @brief Sets all values of the bullet array to NULL and frees them 
 * 
 * @param bullets 
 */
void makeBulletsNull(Bullet * bullets[]);

/**
 * @brief Calls the remove bullets function to remove all out of bounds bullets and then calls the *        move_sprite function to all the bullets
 * 
 * @param bullets       The bullet array
 */
void updateBullets(Bullet * bullets[]);

/**
 * @brief Removes a bullet from the bullet array from a specific index
 * 
 * @param bullets       The bullet array
 * @param bullet_id     Index of the bullet to be removed
 */
void removeBulletById(Bullet * bullets[],unsigned short int bullet_id);

/**
 * @brief Destructs a Bullet object
 * 
 * @param bullet        The bullet to be destroyed
 */
void destroyBullet(Bullet * bullet);

/**@}*/

#endif
