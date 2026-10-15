#ifndef _SPRITE_H_
#define _SPRITE_H_

#include <lcom/lcf.h>

#include <stdint.h>

#include "../font/characters.xpm"

#include "../images/menu/instructions.xpm"
#include "../images/menu/logo.xpm"
#include "../images/menu/play_game.xpm"
#include "../images/menu/quit.xpm"


typedef struct {
  int width;
  int height;
  uint8_t *sprite;
} Sprite;


Sprite characters[CHAR_COUNT];

Sprite menu_instructions;
Sprite menu_logo;
Sprite menu_play_game;
Sprite menu_quit;


/**
 * @brief Loads a xpm_map_t into a Sprite structure
 * @param xpm_map xpm map to be loaded
 * @return loaded Sprite
 */
Sprite load_generic_sprite(xpm_map_t xpm_map);


/**
 * @brief loads the sprites of all characters
 * @return 0 in case of success or 1 in case of error
 */
int load_char_sprites();


/**
 * @brief loads the sprites for all menu elements
 * @return 0 in case of success or 1 in case of error
 */
int load_menu_sprites();

#endif
