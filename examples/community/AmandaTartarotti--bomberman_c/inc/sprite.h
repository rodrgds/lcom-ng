#ifndef _LCOM_SPRITE_H_
#define _LCOM_SPRITE_H_

#include <lcom/lcf.h>
#include "pixmap.h"
#include <minix/sysutil.h>

typedef struct {
int x, y; // current position
int width, height; // dimensions
int xspeed, yspeed; // current speed
//uint8_t *map; // the pixmap
uint32_t *colors;
} Sprite;

Sprite *bloc;
Sprite *bbloc;
Sprite *steven;
Sprite *bomb;
Sprite *explosion;
Sprite *monster;
Sprite *life;
Sprite *lost_life;
Sprite *iblock;
Sprite *up_monster;
Sprite *start_O;
Sprite *start_W;
Sprite *ins_O;
Sprite *ins_W;
Sprite *exit_O;
Sprite *exit_W;
Sprite *title;
Sprite *zero;
Sprite *one;
Sprite *two;
Sprite *three;
Sprite *four;
Sprite *five;
Sprite *six;
Sprite *seven;
Sprite *eight;
Sprite *nine;
Sprite *gameover;
Sprite *gameWin;
Sprite *insGen;
Sprite *unlimited;
Sprite *arrow_mouse;
Sprite *one;
Sprite *black;
Sprite *bar;

/**
     * @brief Read the sprite pixmap, allocate space for it and create a sprite.
     * 
     * @param xpm a xpm_map_t parameter used to generate the sprite image
     * 
     * @return Return the sprite itself
**/
Sprite *create_sprite(xpm_map_t sprite);

/**
     * @brief Creates the game menu sprites depending on active mode (0x115 or 0x14C)
     * 
**/
void create_sprites_menu();

/**
     * @brief Creates the game default sprites depending on active mode (0x115 or 0x14C)
     * 
**/
void create_sprites_game();

/**
     * @brief Free the space of a specific sprite of the game
     * 
     * @param sprite_sp a pointer directed to the memory allocated to the sprite that will be destroyed
**/
void destroy_sprite(Sprite *sp);

/**
     * @brief Free the space related to all sprites of the game
     * 
**/
void destroy_sprites();




#endif
