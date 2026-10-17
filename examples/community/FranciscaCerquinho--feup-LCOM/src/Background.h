#ifndef _BACKGROUND_H
#define	_BACKGROUND_H

#include "Mario.h"
#include "Bitmap.h"
#include "Pipe.h"

/** @defgroup Background Background
 * @{
 *
 * Functions related to the background
 */

typedef struct {
	unsigned int x;
	unsigned int y;
	char *bitmap;

	mario* Mario;

} Background;

/**
 * Initialize the background
 * @param x position x
 * @param y position y
 * @return pointer to the background
 */
Background* newBackground(unsigned int x, unsigned int y);
/**
 * load background
 * @return the new background
 */
Bitmap* loadBackground();
/**
 * Draws the bitmap of the background
 * @param player Pointer to the stuct of the background
 * @param buffer pointer to the video_map
 * @param bmp pointer to the bitmap of the background
 */
void drawBackground(Background* player, char *buffer, Bitmap *bmp);
/**
 * Free the allocated memory
 * @param newBack pointer to the bitmap struct
 */
void DestroyBitmap(Background* newBack);

/** @} */

#endif
