#ifndef ENEMY_H
#define ENEMY_H
typedef struct Enemy Enemy;
#include "gun.h"
#include "sprite.h"
#include "player.h"
#include "game.h"

/** @defgroup enemy 
 * @{
 *
 * Functions related to the enemy
 */

struct Enemy {
    Sprite *sprite;
    int hp, score;
    Gun *gun;

} ;


/**
 * @brief Create an Enemy , at the given x and y, moving towards player
 * 
 * @param game 
 * @param player 
 * @param x 
 * @param y 
 * @return Enemy 
 */
Enemy createEnemy(Game * game, Player *player, int x, int y);

/**
 * @brief Set the Enemy Speed towards player
 * 
 * @param enemy 
 * @param player 
 */
void setEnemySpeed(Enemy *enemy, Player *player);

/**
 * @brief Function to update enemy movement, and shoot. Used every periodic interrupt
 * 
 * @param player 
 */
void enemyAction(Enemy *enemy, Player *player);

/**
 * @brief Move enemy and its gun
 * 
 * @param enemy 
 * @param gameSpeed 
 */
void moveEnemy(Enemy *enemy, double gameSpeed);
#endif

