#ifndef __SHARK_H
#define __SHARK_H

#include "smart_vector.h"

/** @defgroup shark shark
 * @{*/

#include "vector2.h"//not really need because of colision but works as a reminder
#include "collision.h"
#include "sprite.h" //includes "stdlib.h"

struct target{
vector2 position;
vector2 vSpeed;
Sprite* sp;
ColisionRadius colRad;
};

typedef struct target shark;

defineSmartVector(shark);

void createSharks();///<create sharks at beginning of the game
void updateShark(shark *s,char animate);///<update a single shark
void updateSharks(char animate,vector2 scopePos);///<update all sharks
char checkRemoveShark();///<check if a shark should be removed from the current Game
void removeShark(unsigned long index);///<remove shark at index
shark_SV* getSharks();///<get sharks sv
void deleteSharks();///<free allocated memory by sharks

/** @} end of shark */
#endif // __SHARK_H
