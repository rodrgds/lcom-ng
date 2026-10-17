#ifndef __SPRITE_H
#define __SPRITE_H

#include "controller/video_gr/video_gr.h"
#include "position.h"
#include <lcom/lcf.h>

/**
 * @file sprite.h
 * @brief Functions and data structures for creating and managing sprites.
 * 
 * This header defines the sprite_t structure and provides functions for creating,
 * destroying, drawing, and animating sprites.
 */

 /**
  * @struct sprite
  * @brief Represents a sprite, including its position, animation images, and current state.
  *
  * Contains position information, an array of loaded XPM images, 
  * the current frame index, and the total number of images.
  */
struct sprite;
typedef struct sprite sprite_t;

/**
 * @brief Creates a new sprite object from an array of XPM maps.
 * Allocates and initializes a sprite structure with loaded XPM images.
 * The sprite's initial position is set to (x, y), the animation starts at frame 0.
 * 
 * @param pics Array of XPM maps for the sprite animation.
 * @param size Number of images in the pics array. Must be > 0.
 * @param x Initial x-coordinate position of the sprite.
 * @param y Initial y-coordinate position of the sprite.
 * @return Pointer to the newly created sprite_t on success, NULL on failure
 */
sprite_t* create_sprite(xpm_map_t pics[], unsigned int size, uint16_t x, uint16_t y);

/**
 * @brief Destroys a sprite and frees all associated resources.
 * 
 * @param sp Pointer to the sprite to destroy.
 */
void destroy_sprite(sprite_t* sp);

/**
 * @brief Draws the current frame of the sprite at its position.
 * 
 * @param sp Pointer to the sprite to draw.
 * @return 0 if drawing succeeds, non-zero otherwise
 */
int draw_sprite(sprite_t* sp);

/**
 * @brief Draws the current image and advances the animation to the next image.
 * If the last image is reached, the animation loops back to image 0.
 * 
 * @param sp Pointer to the sprite to animate and draw.
 * @return 0 if drawing succeeds, non-zero otherwise.
 */
int draw_sprite_animate(sprite_t* sp);

/**
 * @brief Returns the current position of the sprite.
 * 
 * @param sp Pointer to the sprite.
 * @return Pointer to the position_t structure representing sprite position.
 */
position_t* get_sprite_pos(sprite_t* sp);

/**
 * @brief Gets the height in pixels of the current image of the sprite.
 * 
 * @param sp Pointer to the sprite.
 * @return Height of the current sprite image in pixels.
 */
uint16_t get_sprite_height(sprite_t* sp);

/**
 * @brief Gets the width in pixels of the current image of the sprite.
 * 
 * @param sp Pointer to the sprite.
 * @return Width of the current sprite image in pixels.
 */
uint16_t get_sprite_width(sprite_t* sp);

/**
 * @brief Gets the current animation image index of the sprite.
 * 
 * @param sp Pointer to the sprite.
 * @return Current imgage index.
 */
unsigned int get_sprite_curr(sprite_t* sp);

/**
 * @brief Gets the total number of animation images for the sprite.
 * 
 * @param sp Pointer to the sprite.
 * @return Number of images in the animation.
 */
unsigned int get_sprite_size(sprite_t* sp);

/**
 * @brief Sets the current animation image index of the sprite.
 * 
 * @param sp Pointer to the sprite.
 * @param curr The image index to set as current.
 */
void set_sprite_curr(sprite_t* sp, unsigned int curr);

void set_sprite_flip(sprite_t* sp, bool flip);

#endif //__SPRITE_H
