#ifndef SPRITE_H
#define SPRITE_H
#include <lcom/lcf.h>
#include "videocard.h"


typedef struct {
    uint16_t x, y; // current position
    uint16_t width, height; // dimensions
    uint16_t xspeed, yspeed; // current speed
    uint8_t *map; // the pixmap
} Sprite;

/**
 * @brief Create a sprite object
 * 
 * @param pic the image we want to associate with the sprite
 * @param x horizontal position of the sprite
 * @param y vertical position of the brick
 * @param xspeed horizontal speed of the sprite
 * @param yspeed vertical speed of the sprite
 * @return Sprite* returns a Sprite* object
 */
Sprite* create_sprite(xpm_row_t pic[],uint16_t x,uint16_t y,uint16_t xspeed,uint16_t yspeed);

/**
 * @brief Draws a Sprite
 * 
 * @param sp the sprite we want to draw
 * @return int 0 in case it goes successfully, 1 otherwise
 */
int draw_sprite(Sprite* sp);




/**
 * @brief Destroys a Sprite
 * 
 * @param sp the sprite we want to destroy
 */
void destroy_sprite(Sprite* sp);
#endif
