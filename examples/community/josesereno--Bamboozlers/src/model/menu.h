#ifndef _MENU_H_
#define _MENU_H_

/**
 * @file menu.h
 * @brief Menu screen functions.
 */

#include "sprite.h"
#include "xpm/menu/instructions.xpm"
#include "xpm/menu/levels.xpm"
#include "xpm/menu/logo.xpm"

/** @defgroup menu menu
 * @{
  */

Sprite* levels;
Sprite* logo;
Sprite* instructions;

/**
 * @brief Creates the menu screen sprites.
 *
 * This function initializes various sprites used in the menu screen.
 * The sprites are created using XPM pixel maps.
 */
void (create_menu_sprites)();
/**
 * @brief Destroys the menu screen sprites.
 *
 * This function destroys the sprites created in `create_menu_sprites()`.
 * It releases the memory used by the sprites.
 */
void (destroy_menu_sprites)();

#endif
