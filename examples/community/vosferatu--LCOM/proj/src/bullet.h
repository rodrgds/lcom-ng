#ifndef __BULLET_H
#define __BULLET_H

#include "bitmap.h"
#include "cowboy.h"

/** @defgroup bullet bullet
 * @{
 * 	Bullet functions
 */

/*! Represents the bullet */
typedef struct {
	int x, w, h, vx;
	float y, vy;

	Bitmap* image;

} Bullet;

/**
 * @brief Creates a new bullet, for the specified cowboy ; allocates it
 * @param cowboy Which fired this bullet
 * @param vx X velocity
 * @param vy Y velocity
 * @return A pointer to a bullet type
 */
Bullet* newBullet(Cowboy* cowboy, int vx, float vy);

/**
 * @brief updates the bullet position
 * @param bullet
 */
void updateBullet(Bullet* bullet);

/**
 * @brief draws the bullet on the screen
 * @param bullet
 */
void drawBullet(Bullet* bullet);

/**
 * @brief frees the memory used by bullet
 * @param bullet
 */
void deleteBullet(Bullet* bullet);

/**@}*/


#endif /* _BULLET_H */
