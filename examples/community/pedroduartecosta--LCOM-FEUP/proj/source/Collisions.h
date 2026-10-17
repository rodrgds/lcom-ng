#ifndef __COLLISIONS_H
#define __COLLISIONS_H

/** @defgroup Button Button
 * @{
 * Struct containing the boundaries of the collision object
 */
typedef struct{
	int x1,x2,y1,y2;
} Collisions;

/**
 * @brief creates a collision object
 *
 * @param x coordinate of left top corner
 * @param y coordinate of left top corner
 * @param x coordinate of right bottom corner
 * @param y coordinate of left bottom corner
 * @return Non NULL pointer to the new object
 */
Collisions* newCollision(int x1,int x2,int x3,int x4);

/**
 * @brief verifies collision between player and obstacle
 *
 * @param player
 * @param obstacle
 * @return int telling if there was a collision or not
 */
int checkCollision(Collisions* col1,Collisions* col2); //player no primeiro parametro e obstaculo no segundo

/**
 * @brief deletes collision object
 *
 * @param collision object
 */
void deleteCollision(Collisions* col);

/**@}*/

#endif
