#ifndef _LEVEL_SELECT_H_
#define _LEVEL_SELECT_H_

/**
 * @file level_select.h
 * @brief Level select screen functions.
 */

#include "sprite.h"
#include "xpm/levels/level1.xpm"
#include "xpm/levels/level2.xpm"
#include "xpm/levels/level3.xpm"

/** @defgroup level_select level_select
 * @{
  */
Sprite *level_one;
Sprite *level_two;
Sprite *level_three;

/**
 * @brief Creates the level select screen sprites.
 *
 * This function initializes various sprites used in the level select screen.
 * The sprites are created using XPM pixel maps.
 */
void (create_level_select_sprites)();
/**
 * @brief Destroys the level select screen sprites.
 *
 * This function destroys the sprites created in `create_level_select_sprites()`.
 * It releases the memory used by the sprites.
 */
void (destroy_level_select_sprites)();

#endif
