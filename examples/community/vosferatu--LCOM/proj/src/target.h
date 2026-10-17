#ifndef __TARGET_H
#define __TARGET_H

#include "bitmap.h"

/** @defgroup target target
 * @{
 * 	target functions
 */

/*! Represents the target */
typedef struct {
	int x, y, w, h;

	Bitmap* bullseye;

	int hit;

} Target;

/**
 * @brief creates a new target; allocates it
 * @return a pointer to a target type
 */
Target* newTarget();

/**
 * @brief if the target was hit, give another position at random
 * @param target a pointer to a target type
 */
void updateTarget(Target* target);

/**
 * @brief draws the target on screen
 * @param target a pointer to a target type
 */
void drawTarget(Target* target);

/**
 * @brief frees a target from memory
 * @param target a pointer to a target type
 */
void deleteTarget(Target* target);

/**@}*/


#endif /* _TARGET_H */

