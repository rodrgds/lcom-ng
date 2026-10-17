#ifndef _ENEMY_H
#define _ENEMY_H

#include "Entity.h"

/** @defgroup Enemy Enemy
 * @{
 */
typedef struct enemy_t{
  Entity e;
  int dmg, hp, fr;
} Enemy;
/*
@brief initializes Enemy struct

@param t type of enemy 

@returns an Enemy struct
*/
Enemy enemy(EntityType t);

#endif /*_ENEMY_H*/
