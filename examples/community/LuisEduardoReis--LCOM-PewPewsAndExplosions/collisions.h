#ifndef __COLLISIONS_H_
#define __COLLISIONS_H_

#include "entities.h"


void collision(entity* s, entity* o);

void bruteForceCollisions();

int rectanglesCollision(rectangle a, double ax, double ay, rectangle b, double bx, double by);

int pointToRectangleCollision(double px, double py, rectangle r, double rx, double ry);

#endif
