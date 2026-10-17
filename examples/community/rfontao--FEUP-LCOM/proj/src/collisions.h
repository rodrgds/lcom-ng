#ifndef _COLLISIONS_H_
#define _COLLISIONS_H_

#include <lcom/lcf.h>
#include <lcom/liblm.h>
#include <lcom/proj.h>

#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

#include "sprite.h"
#include "bullet.h"
#include "enemy.h"

/** @defgroup Collisions Collisions
 * @{
 *
 * Functions of the Collisions
 */

/**
 * @brief The cell collision
 */
typedef struct {
        unsigned short int group;       /**<Group of the collidible */
        unsigned short int id;          /**<ID of the collidible */
} CollisionCell;

#define CARGROUP 1              /** @brief Group of the car object*/
#define ALLYBULLETGROUP 2       /** @brief Group of the bullets shot by the car*/
#define ENEMYBULLETGROUP 3      /** @brief Group of the bullets shot by enemies*/
#define ENEMYGROUP 4            /** @brief Group of the enemies*/
#define OBJECTIVEGROUP 5        /** @brief Group of the objective*/       

/**
 * @brief Fills the collision matrix with collision cells with group = 0
 * 
 */
void cleanCollisionMatrix();

/**
 * @brief Draws a collidible on the collision matrix based on the sprite given and the group and ID
 * 
 * @param sprite        Sprite of the collidible to be drawn
 * @param group         Group of the collidible
 * @param id            Id of the collidible
 * @param car           The Car object used for the collision handler
 * @param object        The Objective object used for the collsion handler
 */
void drawCollidible(Sprite * sprite, unsigned short int group, unsigned short int id, Car* car, Objective *object);

/**
 * @brief Handles the collision of two collidibles 
 * 
 * @param collider1     Collision cell of the collider already in the matrix
 * @param collider2     Collision cell of the collider that was going to be drawn
 * @param car           The Car object
 * @param object        The Objective object
 * @return              Used to check if the collidible needs to continue to be drawn to check for *                      other collisions
 */
bool collisionHandler(CollisionCell collider1,CollisionCell collider2, Car* car, Objective* object);

/**@}*/

#endif
