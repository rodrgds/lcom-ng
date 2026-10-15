/**
 * @file enemy.h
 * @brief Contains functions for handling enemy entities in the game.
 */

#ifndef ENEMY_H_
#define ENEMY_H_

#include <lcom/lcf.h>
#include <stdint.h>
#include "game/sprite/sprite.h"
#include "game/classes/enemy_bullet.h"


/**
 * @brief Struct representing an enemy entity in the game.
 * 
 * Stores information about the enemy's attributes and behavior.
 */
typedef struct
{
    int life;
    int damage;
    int x;
    int y;
    int speed_x;
    int speed_y;
    Sprite *sprite;
    bool alive;
    enemy_bullet *shot;
} enemy;


/**
 * @brief Creates a new enemy.
 * 
 * @param life The initial life points of the enemy.
 * @param damage The damage dealt by the enemy.
 * @param x The initial x-coordinate of the enemy.
 * @param y The initial y-coordinate of the enemy.
 * @param speed_x The horizontal speed of the enemy.
 * @param speed_y The vertical speed of the enemy.
 * @param sp Pointer to the sprite associated with the enemy.
 * @param alive Indicates if the enemy is initially alive.
 * @return A pointer to the newly created enemy.
 */
enemy *createEnemy(int life,int damage,int x,int y,int speed_x,int speed_y, Sprite *sp,bool alive);


/**
 * @brief Creates a new enemy with a default bullet.
 * 
 * This function is a convenience wrapper around createEnemy.
 * 
 * @param life The initial life points of the enemy.
 * @param damage The damage dealt by the enemy.
 * @param x The initial x-coordinate of the enemy.
 * @param y The initial y-coordinate of the enemy.
 * @param speed_x The horizontal speed of the enemy.
 * @param speed_y The vertical speed of the enemy.
 * @param sp Pointer to the sprite associated with the enemy.
 * @param alive Indicates if the enemy is initially alive.
 * @return A pointer to the newly created enemy.
 */
enemy *createEnemyWithDefaultShot(int life, int damage, int x, int y, int speed_x, int speed_y, Sprite *sp, bool alive);



/**
 * @brief Destroys an enemy.
 * 
 * @param p Pointer to the enemy to be destroyed.
 */
void destroyEnemy(enemy *p);


/**
 * @brief Revives an enemy at the specified position.
 * 
 * @param p Pointer to the enemy to be revived.
 * @param x The x-coordinate of the revival position.
 * @param y The y-coordinate of the revival position.
 */
void reviveEnemy(enemy *p,int x,int y);

#endif
