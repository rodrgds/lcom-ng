#ifndef ANIMATE_SPRITE_H
#define ANIMATE_SPRITE_H

#include <stdarg.h> // va_* macros are defined here
#include "sprite.h"
#include "ball.h"


typedef struct {
    Sprite *sp; // standard sprite
    int aspeed; // no. frames per pixmap
    int cur_aspeed; // no. frames left to next change
    int num_fig; // number of pixmaps
    int cur_fig; // current pixmap
    uint8_t** map;
}AnimSprite;


/**
 * @brief Create an AnimSprite object
 * 
 * @param x horizontal position of the brick
 * @param y vertical position of the brick
 * @param no_pic the number of images that will be used in the animation
 * @param pic1 the type of the pics that we will use in the animation
 * @return AnimSprite* 
 */
AnimSprite *create_animSprite(uint16_t x, uint16_t y, uint8_t no_pic, xpm_row_t pic1[], ...);

/**
 * @brief processes the animation of an AnimSprite
 * 
 * @param asp the AnimSprite that will be animated
 * @return int 0 in case it goes successfully, one otherwise
 */
int animate_sprite(AnimSprite *asp);

/**
 * @brief destroys an already created AnimSprite
 * 
 * @param asp the sprite that we're going to destroy
 */
void destroy_animSprite(AnimSprite* asp);

#endif
