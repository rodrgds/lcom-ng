#ifndef __COWBOY_H
#define __COWBOY_H

#include "bitmap.h"

/** @defgroup cowboy cowboy
 * @{
 * 	Cowboy functions
 */

/*! Represents the cowboy */
typedef struct {
	int x, y, w, h;

	Bitmap* alive;
	Bitmap* shooting;
	Bitmap* dead;

	int state, opponent;

} Cowboy;

/**
 *	@brief Creates a new cowboy ; allocates it
 * @param opponent -> if the cowboy is AI, opponent = 1
 * @return a pointer to a cowboy type
 */
Cowboy* newCowboy(unsigned int opponent);

/**
 * @brief Updates the cowboy position
 * @param cowboy Cowboy to be updated
 * @param hor Can be 0, 1 or -1. movement in horizontal direction
 * @param vert Can be 0, 1 or -1. movement in vertical direction
 */
void updateCowboy(Cowboy* cowboy, int hor, int vert);

/**
 * @brief Draws the cowboy bitmap according to its state
 * @param cowboy
 */
void drawCowboy(Cowboy* cowboy);

/**
 * @brief Frees from memory
 * @param cowboy
 */
void deleteCowboy(Cowboy* cowboy);

/**@}*/

#endif /* _COWBOY_H */
