#ifndef SPRITE_H
#define SPRITE_H

#include <lcom/lcf.h>
#include "../../../drivers/graphic/include/graphic.h"

typedef struct sprite{
  int x, y; 
  int width, height; 
  int xspeed, yspeed; 
  xpm_image_t* img; 
} sprite_t;

/**
 * @brief Generates a new sprite, with the help of a xpm
 * 
 * @param xpm XPM file that will support the new sprite's creation.
 * @param x initial horizontal coordinate.
 * @param y initial vertical coordinate.
 * @return sprite_t* Points to the sprite created.
 */
sprite_t* sprite_new(const char* const* xpm, int x, int y); 

/**
 * @brief Deletes a created sprite.
 * 
 * @param sprite Sprite to be deleted.
 */
void sprite_delete(sprite_t* sprite); 

/**
 * @brief Returns the sprite's width.
 * 
 * @param sprite Sprite that will retrieve its width.
 * @return int with's value.
 */
int sprite_get_width(sprite_t* sprite); 

/**
 * @brief Returns the sprite's height.
 * 
 * @param sprite Sprite that will retrieve its height.
 * @return int height's value.
 */
int sprite_get_height(sprite_t* sprite); 

/**
 * @brief Returns the sprite's horizontal speed.
 * 
 * @param sprite Sprite that will retrieve its horizontal speed.
 * @return int horizontal speed's value.
 */
int sprite_get_xspeed(sprite_t* sprite); 

/**
 * @brief Returns the sprite's vertical speed.
 * 
 * @param sprite Sprite that will retrieve its vertical speed.
 * @return int vertical speed's value.
 */
int sprite_get_yspeed(sprite_t* sprite); 

/**
 * @brief Returns the horizontal coordinate.
 * 
 * @param sprite Sprite that will retrieve its horizontal coordinate.
 * @return int horizontal's value.
 */
int sprite_get_x(sprite_t* sprite); 

/**
 * @brief Returns the vertical coordinate.
 * 
 * @param sprite Sprite that will retrieve its horizontal coordinate.
 * @return int horizontal's value.
 */
int sprite_get_y(sprite_t* sprite); 

/**
 * @brief Returns the sprite's image.
 * 
 * @param sprite Sprite that will retrieve its images.
 * @return xpm_image_t* XPM's image of the sprite.
 */
xpm_image_t* sprite_get_image(sprite_t* sprite); 

/**
 * @brief Draws a sprite in the screen.
 * 
 * @param sprite Sprite to be drawn.
 */
void sprite_draw(sprite_t* sprite); 

#endif

