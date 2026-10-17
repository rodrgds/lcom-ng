#ifndef SPRITE_H
#define SPRITE_H

#include "../../../devices/graphics/graphics.h"
#include "../../context.h"
#include <lcom/lcf.h>


///@brief Builds a sprite from an XPM image.
///@param sprite Pointer to the sprite structure to be initialized.
///@param xpm_ The XPM image data.
///@return 0 if successful, 1 otherwise.
int sprite_build1(sprite_t* sprite, xpm_map_t xpm_);

///@brief Deletes a sprite, freeing its associated resources.
///@param sprite Pointer to the sprite to be deleted.
void (sprite_delete)(sprite_t * sprite);

///@brief Sets the x-coordinate of a sprite's position.
///@param sprite Pointer to the sprite.
///@param x The x-coordinate value.
void set_sprite_xpos(sprite_t *sprite, uint16_t x);

///@brief Sets the y-coordinate of a sprite's position.
///@param sprite Pointer to the sprite.
///@param y The y-coordinate value.
void set_sprite_ypos(sprite_t *sprite, uint16_t y);

///@brief Gets the x-coordinate of a sprite's position.
///@param sprite Pointer to the sprite.
///@return The x-coordinate value.
uint16_t get_sprite_xpos(sprite_t *sprite);

///@brief Gets the y-coordinate of a sprite's position.
///@param sprite Pointer to the sprite.
///@return The y-coordinate value.
uint16_t get_sprite_ypos(sprite_t *sprite);

#endif
