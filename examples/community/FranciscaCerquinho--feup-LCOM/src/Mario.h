#ifndef _MARIO_H
#define	_MARIO_H


/** @defgroup Mario Mario
 * @{
 * Functions related to the Mario methods
 */


#include "Bitmap.h"



typedef struct {
	unsigned int x;
	unsigned int y;
	Bitmap *imageOne, *imageTwo;
} mario;

/**
 * Initialize the mario
 * @param x position x
 * @param y position y
 * @return pointer to the mario struct
 */
mario* newMario(unsigned int x, unsigned int y);

/**
 * loads the mario bitmap
 * @return mario bitmap pinter
 */
Bitmap* loadMario();


/**
 * draws the bitmap
 * @param player pointer to the mario stuct
 * @param buffer pointer to the video_map
 * @param bmp pointer to the mario bitmap
 */
void drawMario(mario* player, char *buffer, Bitmap *bmp);

/** @} */

#endif
