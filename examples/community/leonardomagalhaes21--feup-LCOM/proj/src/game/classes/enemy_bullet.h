/**
 * @file enemy_bullet.h
 * @brief Contains functions for handling enemy bullets fired by enemies.
 */

#ifndef ENEMY_BULLET_H_
#define ENEMY_BULLET_H_

#include <lcom/lcf.h>
#include <stdint.h>
#include "game/sprite/sprite.h"

/**
 * @brief Struct representing an enemy bullet fired by an enemy.
 * 
 * Stores information about the bullet's position, velocity, damage, and associated sprite.
 */
typedef struct
{
    int x;
    int y;
    int playerX;
    int playerY;
    int damage;
    float dx;
    float dy;
    Sprite *sprite;
    bool active;

} enemy_bullet;


/**
 * @brief Creates a new enemy bullet.
 * 
 * @param x The initial x-coordinate of the bullet.
 * @param y The initial y-coordinate of the bullet.
 * @param playerX The x-coordinate of the player character.
 * @param playerY The y-coordinate of the player character.
 * @param damage The damage inflicted by the bullet.
 * @param sp Pointer to the sprite associated with the bullet.
 * @return A pointer to the newly created enemy bullet.
 */
enemy_bullet *createEnemyBullet( int x, int y,int playerX, int playerY,int damage,Sprite *sp);


/**
 * @brief Destroys an enemy bullet.
 * 
 * @param p Pointer to the enemy bullet to be destroyed.
 */
void destroyEnemyBullets(enemy_bullet *p);


/**
 * @brief Moves an enemy bullet based on its velocity.
 * 
 * @param p Pointer to the enemy bullet to be moved.
 */
void moveEnemyBullet(enemy_bullet *p);
#endif
