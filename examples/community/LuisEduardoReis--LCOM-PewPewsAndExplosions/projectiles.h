#ifndef __PROJECTILES_H_
#define __PROJECTILES_H_

#include "entities.h"
#include "graphics.h"
#include "sprites.h"

void initBullet(entity* self);
void tickBullet(entity* self);
void drawBullet(entity* self);
void destroyBullet(entity* self);

void initEBullet(entity* self);
void tickEBullet(entity* self);
void drawEBullet(entity* self);
void destroyEBullet(entity* self);

void initExplosion(entity* self);
void tickExplosion(entity* self);
void drawExplosion(entity* self);
void destroyExplosion(entity* self);



#endif
