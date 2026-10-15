#ifndef BULLET_H_
#define BULLET_H_

#include <lcom/lcf.h>
#include <stdint.h>
#include "game/sprite/sprite.h"


/**
 * @brief Struct representing a bullet fired by a player.
 * 
 * Stores information about the bullet's position, velocity, damage, and associated sprite.
 */
typedef struct {
    int x; 
    int y; 
    int mouseX;
    int mouseY;
    int damage;
    float dx;
    float dy;
    Sprite *sprite;
} bullet;

/**
 * @brief Creates a new bullet.
 * 
 * @param x The initial x-coordinate of the bullet.
 * @param y The initial y-coordinate of the bullet.
 * @param mouseX The x-coordinate of the mouse pointer.
 * @param mouseY The y-coordinate of the mouse pointer.
 * @param damage The damage inflicted by the bullet.
 * @param sp Pointer to the sprite associated with the bullet.
 * @return A pointer to the newly created bullet.
 */
bullet *createBullet(int x,int y, int mouseX, int mouseY,int damage,Sprite *sp);


/**
 * @brief Destroys a bullet if it is out of bounds.
 * 
 * @param p Pointer to the bullet to be destroyed.
 */
void destroyBullets(bullet *p);


/**
 * @brief Moves a bullet based on its velocity.
 * 
 * @param p Pointer to the bullet to be moved.
 */
void moveBullet(bullet *p);
#endif
