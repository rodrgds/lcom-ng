#ifndef __bomb_h
#define __bomb_h



#include "Object.h"
#include "xpm/bomb.xpm"
#include "xpm/explode.xpm"
#include "xpm/bala.xpm"
#include <math.h>
#include "classes/Plane.h"
#include "classes/Player.h"
#include "classes/Score.h"

typedef struct {
    Object** bombs;
    int sizeBombs;
} Bombs;

Bombs *create_bombsArray(int maxBombNumber);

void (createBomb)(Bombs *b, int x, int y, bool down);

void (removeBomb)(Bombs *b, int indexToRemove);

int (bombMovement)(Bombs *bombsArray, int count);

void (bulletMovement)(Bombs *bombsArray, Planes *planesArray);

void (drawBombs)(Bombs *bombsArray);

void (drawBullets)(Bombs *bulletArray);

int bombCollision(Bombs *bombsArray, Player *player);

#endif
