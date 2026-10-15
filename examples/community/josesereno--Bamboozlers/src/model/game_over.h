#ifndef _GAME_OVER_H_
#define _GAME_OVER_H_

/**
 * @file game_over.h
 * @brief Game over screen functions.
 */

#include "sprite.h"
#include "xpm/game_over/exit.xpm"
#include "xpm/game_over/play_again.xpm"
#include "xpm/game_over/game_over.xpm"

/** @defgroup game_over game_over
 * @{
 */

Sprite *game_over;
Sprite *play_again;
Sprite *game_exit;

/**
 * @brief Creates the game over screen sprites.
 */
void (create_game_over_sprites)();
/**
 * @brief Destroys the game over screen sprites.
 */
void (destroy_game_over_sprites)();



#endif
