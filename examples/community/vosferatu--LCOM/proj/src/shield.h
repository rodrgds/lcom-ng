#ifndef __SHIELD_H
#define __SHIELD_H

#include "bitmap.h"

/** @defgroup shield shield
 * @{
 * 	shield functions
 */

/*! Represents the shield */
typedef struct {
	int x, y, w, h, vy;

	Bitmap* form;

} Shield;

/**
 * @brief creates a new shield ; allocates it
 * @param cactus if the shield drawing is a cactus (1) else is a car
 * @return a pointer to a shield type
 */
Shield* newShield(unsigned int cactus);

/**
 * @brief updates the shield position
 * @param shield a pointer to a shield type
 */
void updateShield(Shield* shield);

/**
 * @brief draws the shield on the screen
 * @param shield a pointer to a shield type
 */
void drawShield(Shield* shield);

/**
 * @brief frees the shield from memory
 * @param shield a pointer to a shield type
 */
void deleteShield(Shield* shield);

/**@}*/


#endif /* _SHIELD_H */
