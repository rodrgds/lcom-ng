#ifndef SPRITE_H
#define SPRITE_H

#include <lcom/lcf.h>

typedef struct {
  int width;
  int height;
  uint8_t *map;
} sprite;

/**
 * @brief Create a new sprite
 * @param map xpm map to generate sprite from
 * @return pointer to newly allocated sprite
*/
sprite* create_sprite(xpm_map_t map);

/**
 * @brief Destroy a sprite
 * @param sp sprite to be destroyed
*/
void destroy_sprite(sprite *sp);

#endif
