#ifndef __SPRITE_H
#define __SPRITE_H

#include <lcom/lcf.h>
#include "../drivers/graphics/graphics.h"

#include "../xpm/cursor.xpm"
#include "../xpm/background.xpm"
#include "../xpm/logo.xpm"
#include "../xpm/single_player.xpm"
#include "../xpm/single_playerh.xpm"
#include "../xpm/coop.xpm"
#include "../xpm/cooph.xpm"
#include "../xpm/exit.xpm"
#include "../xpm/exitH.xpm"
#include "../xpm/fireboy.xpm"
#include "../xpm/watergirl.xpm"
#include "../xpm/game-over.xpm"
#include "../xpm/pause.xpm"
#include "../xpm/waiting.xpm"

typedef struct {
    int x, y, xspeed, yspeed;
    uint16_t width, height;
    uint32_t* map;
} Sprite;

Sprite* background;
Sprite* cursor;
Sprite* logo;
Sprite* single_player_text;
Sprite* coop_text;
Sprite* exit_text;
Sprite* single_player_texth;
Sprite* coop_texth;
Sprite* exit_texth;
Sprite* block;
Sprite* fzl;
Sprite* game_over;
Sprite * pause_menu;
Sprite * waiting;

/**
 * @brief Creates a sprite object.
 *
 * @param pic The xpm image.
 * @param x The horizontal position of the sprite.
 * @param y The vertical position of the sprite.
 * @param xspeed The horizontal speed of the sprite.
 * @param yspeed The vertical speed of the sprite.
 * @return Sprite* The created sprite.
 */
Sprite* create_sprite(xpm_map_t pic, uint16_t x, uint16_t y, uint16_t xspeed, uint16_t yspeed);

/**
 * @brief Loads all the sprites needed.
 * @return 1 if a sprite fails to load, 0 otherwise
 */
int load_sprites();

/**
 * @brief Destroys a sprite.
 *
 * @param sp The sprite to destroy.
 * @return int 1 if the sprite is null, 0 otherwise.
 */
int destroy_sprite(Sprite* sp);

/**
 * @brief Draws a sprite.
 *
 * @param sp The sprite to be drawn.
 * @return int 1 if the sprite does not exist, 0 otherwise.
 */
int draw_sprite(Sprite* sp);

/**
 * @brief Erases a sprite from the screen.
 *
 * @param sp The sprite to be erased.
 * @return int 1 if the sprite does not exist, 0 otherwise.
 */
int erase_sprite(Sprite* sp);

#endif /* __SPRITE_H */
