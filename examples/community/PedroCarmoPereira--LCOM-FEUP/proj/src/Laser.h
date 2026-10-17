#ifndef _LASER_H
#define _LASER_H

#include "Entity.h"

/** @defgroup Laser Laser
 * @{
 */

typedef struct laser_t
{
  Entity e;
  float angle;
  int dmg;
  bool friendly;
} Laser;

typedef struct  laserVector{
  Laser curr;
  struct laserVector * next;
} * LaserVector;

/*
@brief initializes laser struct

@param xi to calculate angle for player's laser

@param yi to calculate angle for player's laser

@param xf to calculate angle for player's laser

@param yf to calculate angle for player's laser

@param dmg how much damge the laser does

@param friendly true if the laser is from the player, false if it is from the enemies

@param angle angle of the laser

@return Created Laser
*/

Laser laser(int xi, int yi, int xf, int yf, int dmg, bool friendly, float angle);

/*
@brief erases laser

@param v vector of the lasers
*/

void freeLasers(LaserVector * v);

/*
@brief adds a new laser

@param v vector of the lasers

@param l laser to add
*/

void addLaser(LaserVector *v, Laser *l);

#endif
