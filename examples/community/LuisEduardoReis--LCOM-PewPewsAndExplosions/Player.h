#ifndef __PLAYER_H_
#define __PLAYER_H_

#include "entities.h"
#include "graphics.h"
#include "sprites.h"

void initPlayer(entity* self);
void tickPlayer(entity* self);
void drawPlayer(entity* self);
void destroyPlayer(entity* self);



#endif
