#ifndef _LCOM_SPRITE_H_
#define _LCOM_SPRITE_H_

#include "controller/graphics/graphics.h"

/**
 * @brief Struct that represents a sprite and its information.
*/
typedef struct {
    uint16_t height;
    uint16_t width;
    uint32_t *map;
    uint32_t color;
} Sprite;

/**
 * @brief Function that creates a sprite.
 * @param sprite xpm_map_t with the sprite information.
 * @return Pointer to the sprite.
*/
Sprite* (create_sprite)(xpm_map_t sprite);

/**
 * @brief Function that destroys a sprite.
 * @param sprite Struct with the sprite information.
*/
void (destroy_sprite)(Sprite *sprite);

#endif
