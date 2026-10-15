#ifndef _MODEL_H_
#define _MODEL_H_

/**
 * @file model.h
 * @brief Sprites creation and destruction functions.
 */

#include "menu.h"
#include "game.h"
#include "instructions.h"
#include "level_select.h"
#include "game_over.h"
#include "xpm/cursor.xpm"

/** @defgroup model model
 * @{
  */
Sprite* cursor;

/**
 * @brief Creates all sprites used in the game.
 *
 * This function calls individual sprite creation functions to create
 * sprites for the mouse, menu, game, instructions, game over, and level select screens.
 */
void (create_sprites)();
/**
 * @brief Destroys all sprites used in the game.
 *
 * This function calls individual sprite destruction functions to destroy
 * sprites created in the `create_sprites()` function. It also destroys the mouse cursor sprite.
 */
void (destroy_sprites)();
/**
 * @brief Creates the mouse cursor sprites.
 *
 * This function creates sprites for the mouse cursor in different states,
 * such as normal, hand, and dragging. The sprites are created using XPM pixel maps.
 */
void (create_mouse_sprites)();

#endif
