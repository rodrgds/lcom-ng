#ifndef _ENEMY_H_
#define _ENEMY_H_


#include <lcom/lcf.h>
#include <lcom/liblm.h>
#include <lcom/proj.h>

#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>

#include "sprite.h"
#include "collisions.h"

/** @defgroup Enemy Enemy
 * @{
 *
 * Constants and functions of the enemies
 */

#define MAX_ENEMY_COUNT 	20	/**< @brief Maximum number of enemies present in game */ 
#define ENEMY_GUN_COOLDOWN 	50	/**< @brief Cooldown between enemy shots */
#define ENEMY_HP_INITIAL 	60	/**< @brief Healths points of each enemy */
#define INITIAL_ENEMY_COUNT     10 	/**< @brief Initial maximum number of enemies present in 					the game (round 1) */
/**
 * @brief Enemy Struct
 * 
 */
typedef struct
{
	Sprite *sprite;			/**< @brief Enemy sprite */
	int hp;				/**< @brief Current health points of the enemy */
	unsigned int id;		/**< @brief Id of the enemy */
	bool shootingCar;		/**< @brief True if the enemy is shooting the Car */
	bool shootingObj;		/**< @brief True if the enemy is shooting the Objective*/
	unsigned int gun_cooldown;	/**< @brief Cooldown between enemy shots */
} Enemy;


/**
 * @brief Spawns an enemy on one of the designated spawn spots. Allocates memory for an enemy and *	  places it in the Enemy array in the first non NULL position
 * 
 */
void spawnEnemy();

/**
 * @brief Draws all the enemies and their collidibles from the Enemy array 
 * 
 * @param car 		The Car object
 * @param obj 		The Objective object
 */
void drawEnemies(Car *car, Objective* obj);

/**
 * @brief Removes and frees an enemy in a certain index of the Enemy array
 * 
 * @param enemy_id 	Index of the enemy to be removed
 */
void killEnemy(unsigned short int enemy_id);

/**
 * @brief Handles the behaviour of the enemies. If the car is close enemies shoot the car. If the *	  objective is close they shoot the objective. Else the walk towards the objective.
 * 
 * @param car 		The Car object
 * @param obj 		The Objective object
 * @param enemyAnim 	The Enemy Sprite array for the animations
 */
void updateEnemies(Car *car, Objective *obj, Sprite *enemyAnim[]);

/**
 * @brief Animates an enemy by choosing a sprite from the array based on the desired facing of the *        enemy
 * 
 * @param enemy		The object of the Enemy to be animated 
 * @param enemyAnim 	The Enemy Sprite array for the animations
 */
void animEnemy(Enemy *enemy, Sprite *enemyAnim[]);

/**@}*/

#endif
