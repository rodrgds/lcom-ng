/**
 * @file sprite
 * @brief Functions related with the sprites of the game
 */

#ifndef _LCOM_SPRITE_H_
#define _LCOM_SPRITE_H_

/** @defgroup Sprite Sprite
 * @{
 *
 * Functions related with the sprites of the game
 */

#include "../../devices/video_card/graphics.h"
#include "../background/background_xpm.h"
#include "../mouse/mouse_xpm.h"
#include "../title/title_xpm.h"
#include "../menu/menu_option_xpm.h"
#include "../symbol/symbol_xpm.h"
#include "../letter/letter_xpm.h"
#include "../number/number_xpm.h"


typedef struct {
  int width, height;
  uint32_t *map;
} Sprite;

/**
 * @brief Creates a sprite in memory
 *
 * @param xmap The xpm image to create the sprite
 * @return Return a pointer to the created sprite, if invalid pixmap return null
 */
Sprite *create_sprite(xpm_map_t xmap);

/**
 * @brief Destroy the sprite freeing its memory
 * 
 * @param sp A pointer to the sprite that need to be destroyed
 */
void destroy_sprite(Sprite *sp);

/**
 * @brief Display the sprite on the screen
 *
 * @param sprite The sprite that will be displayed
 * @param x The x position of the upper left corner of the sprite image
 * @param y The y position of the upper left corner of the sprite image
 * @return Return 0 upon success and 1 otherwise
 */
int display_sprite(Sprite *sprite, int x, int y);

/**@}*/

#endif
