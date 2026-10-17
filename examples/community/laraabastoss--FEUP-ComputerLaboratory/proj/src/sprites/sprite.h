#ifndef _LCOM_SPRITE_H_
#define _LCOM_SPRITE_H_

#include "devices/video/graphics.h"

typedef struct {
    uint16_t height;
    uint16_t width;
    uint32_t *map;
} Sprite; 

/// @brief alocates the neccessaery memory for a Sprite
/// @param pic the xmp that represents the Sprite
/// @return the created Sprite
Sprite *create_sprite(xpm_map_t pic);

/// @brief detroys a Sprite
/// @param sp the Sprite that will be destroyed
void destroy_sprite(Sprite *sp);

#endif
