#ifndef __ENEMIES_H_
#define __ENEMIES_H_

#include "entities.h"

void initEnemy(entity* self);
void tickEnemy(entity* self);
void drawEnemy(entity* self);
void destroyEnemy(entity* self);


void initEnemyCruiser(entity* self);
void tickEnemyCruiser(entity* self);
void drawEnemyCruiser(entity* self);
void destroyEnemyCruiser(entity* self);

void initEnemyMotherShip(entity* s);
void tickEnemyMotherShip(entity* s);
void drawEnemyMotherShip(entity* s);
void destroyEnemyMotherShip(entity* s);

#endif
