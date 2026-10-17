#ifndef _COLISION_H
#define	_COLISION_H

/** @defgroup Collision Collision
 * @{
 * Functions related to the collision methods
 */

#include "Bitmap.h"
#include "Mario.h"
#include "Pipe.h"

/**
 * Detects the colsion between  the pipe and mario
 * @param newmario pointer to the mario struct
 * @param pipe pointer to the Pipe struct
 */
int ColisionPipe(mario *newmario, Pipe *pipe);

/** @} */

#endif
