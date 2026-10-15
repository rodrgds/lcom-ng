#ifndef _GAME_H_
#define _GAME_H_

/**
 * @file game.h
 * @brief Game logic and sprite creation/destruction functions.
 */

#include "sprite.h"
#include "xpm/game/panda.xpm"
#include "xpm/game/background.xpm"
#include "xpm/game/block.xpm"
#include "xpm/game/plank.xpm"
#include "xpm/home.xpm"
#include "xpm/numbers.xpm"
#include "xpm/game/lava.xpm"
#include "xpm/game/map.xpm"
#include "xpm/game/bamboo.xpm"

/** @defgroup game game
 * @{
  */
Sprite *home;
Sprite *background;
Sprite *little_block;
Sprite *big_block;
Sprite *little_plank;
Sprite** map_items;
Sprite *big_plank;
Sprite* lava;
Sprite* map;
Sprite *panda;
Sprite *bamboo;
Sprite *timer[4];

/**
 * @brief Creates the game sprites.
 *
 * This function initializes various sprites used in the game.
 * The sprites are created using XPM pixel maps.
 */
void (create_game_sprites)();

/**
 * @brief Destroys the game sprites.
 *
 * This function destroys the sprites created in `create_game_sprites()`.
 * It releases the memory used by the sprites.
 */
void (destroy_game_sprites)();

#endif
