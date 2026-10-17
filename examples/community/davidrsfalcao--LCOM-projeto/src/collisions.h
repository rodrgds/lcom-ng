#ifndef __COLLISIONS_H
#define __COLLISIONS_H

#include "asteroid.h"
#include "space_ship.h"
/**
 * @brief verifies if the spaceship collides with the asteroids
 * @param nv1 the spaceship
 * @param ast1 one of the asteroids
 * @param ast2 one of the asteroids
 * @return 1 if the spaceship has collided or 0 if not
 */
unsigned int collision_spaceship_asteroid(space_ship nv1, asteroid ast1, asteroid ast2);

#endif
