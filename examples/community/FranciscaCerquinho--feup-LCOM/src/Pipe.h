#ifndef _Pipe_H
#define	_Pipe_H


/** @defgroup Pipe Pipe
 * @{
 * Functions related to the pipe methods
 */

#include "Bitmap.h"
#include "Background.h"

/*
 * Struct with the pipes position and image
 */
typedef struct {
	unsigned int x;
	unsigned int y;
	Bitmap *image;
} Pipe;

/*
 * Inicialize the pipe
 * @param x position x of the pipe
 * @param y position y of the pipe
 * @return pipe struct
 */
Pipe* newPipe(unsigned int x, unsigned int y);

/**
 * Load the pipe
 * @return pipe
 */
Bitmap* loadPipe();

/**
 * Draw the pipe
 * @param player pipe
 * @param buffer buffer
 * @param bmp pipe bitmap
 */
void drawPipe(Pipe* player, char *buffer, Bitmap *bmp);

/** @} */

#endif
