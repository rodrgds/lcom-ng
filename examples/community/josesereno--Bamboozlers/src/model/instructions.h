#ifndef _INSTRUCTIONS_H_
#define _INSTRUCTIONS_H_

/**
 * @file instructions.h
 * @brief Instruction screen functions.
 */

#include "sprite.h"
#include "xpm/instructions_text.xpm"

/** @defgroup instructions instructions
 * @{
  */

Sprite *instructions_text_1;
Sprite *instructions_text_2;
Sprite *instructions_text_3;
Sprite *instructions_text_4;
Sprite *instructions_text_5;
Sprite *instructions_text_6;

/**
 * @brief Creates the instruction screen sprites.
 *
 * This function initializes various sprites used in the instruction screen.
 * The sprites are created using XPM pixel maps.
 */
void (create_instructions_sprites)();

/**
 * @brief Destroys the instruction screen sprites.
 *
 * This function destroys the sprites created in `create_instructions_sprites()`.
 * It releases the memory used by the sprites.
 */
void (destroy_instructions_sprites)();

#endif
