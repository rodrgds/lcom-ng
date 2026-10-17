#ifndef __COLISION_H
#define __COLISION_H

#include "vector2.h"

/** @defgroup collision collision
 * @{*/


typedef float ColisionRadius;
/**
 *
 * @param v1 circle1 center coordinates
 * @param r1 circle1 collision radius
 * @param v2 circle2 center coordinates
 * @param r2 circle1 collision radius
 * @return 0 if collision does not occur, something else otherwise
 */
char circlesColide(vector2 v1, ColisionRadius r1,vector2 v2,ColisionRadius r2);

char pointInsideCircle(vector2 point, vector2 circleCenter,ColisionRadius r1);

/** @} end of collision */
#endif //__COLISION_H
