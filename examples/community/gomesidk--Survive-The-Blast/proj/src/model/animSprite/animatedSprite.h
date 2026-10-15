#ifndef __ANIMATED_SPRITE_H
#define __ANIMATED_SPRITE_H

#include <lcom/lcf.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdarg.h>
#include "sprite.h"

/**
 * @file animatedSprite.h
 * @brief Animated sprite management for the game
 * 
 * This module provides functions to create, destroy, and draw animated sprites.
 * It allows for the management of multiple frames of animation, including their dimensions and speed.
 */

struct animatedSprite_imp;
typedef struct animatedSprite_imp AnimSprite;

/**
 * @brief Returns the width of the animated sprite
 * 
 * @param sp Pointer to the animated sprite
 * @return The width of the animated sprite
 */
uint16_t get_anim_sprite_width(AnimSprite *sp);

/**
 * @brief Returns the height of the animated sprite
 * 
 * @param sp Pointer to the animated sprite
 * @return The height of the animated sprite
 */
uint16_t get_anim_sprite_height(AnimSprite *sp);

/**
 * @brief Returns the animation speed of the animated sprite
 * 
 * @param sp Pointer to the animated sprite
 * @return The animation speed of the animated sprite
 */
uint8_t get_anim_sprite_aspeed(AnimSprite *sp);

/**
 * @brief Returns the number of images in the animated sprite
 * 
 * @param sp Pointer to the animated sprite
 * @return The number of images in the animated sprite
 */
uint8_t get_anim_sprite_num_images(AnimSprite *sp);


/**
 * @brief Creates a new animated sprite from XPM "pic"
 * This function allocates memory for the animated sprite and loads the XPM images.
 * It does not draw the animated sprite on the screen.
 * 
 * @param no_pic The number of images in the animation
 * @param pic1 The first XPM image to be loaded
 * @param ... The rest of the XPM images to be loaded
 * @return Pointer to the created animated sprite, or NULL if the pixmap is invalid
 */
AnimSprite *create_anim_sprite(uint8_t anim_speed, uint8_t no_pic, xpm_map_t pic1, ...);

/**
 * @brief Frees the memory allocated for the animated sprite
 * 
 * @param sp Pointer to the animated sprite to be destroyed
 */
void destroy_anim_sprite(AnimSprite *sp);

/**
 * @brief Draws the animated sprite on the screen at the specified coordinates
 * 
 * @param sp Pointer to the animated sprite to be drawn
 * @param x The x-coordinate where the animated sprite will be drawn
 * @param y The y-coordinate where the animated sprite will be drawn
 * @param current_image The current frame of the animation
 */
void draw_anim_sprite(AnimSprite *sp, uint16_t x, uint16_t y, uint8_t current_image);

/** @} end of sprite */

#endif /* __ANIMATED_SPRITE_H */
