/**
 * @file sprites.h
 * @brief file used to create and destroy sprites
 * 
 */

#ifndef _SPRITES_H_
#define _SPRITES_H_

#include <lcom/lcf.h>
#include <sys/types.h>
#include "devices/video/video.h"

typedef struct Sprite{
    uint16_t height, width;
    uint32_t *bitmap, color;
} Sprite; 


/**
 * @brief this class adpated function creates a sprite object
 * 
 * @param sprite xpm_map that will be converted in an object of the type struct Sprite
 * @return Sprite*: a pointer to the converted Sprite 
 */
Sprite *createSprite(xpm_map_t sprite);


/**
 * @brief this class adapted function frees the memory that the sprite occupated
 * 
 * @param sp sprite that we want to free 
 */
void destroySprite(Sprite *sp);

#endif /* _SPRITES_H_ */
