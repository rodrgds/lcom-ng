#ifndef _SRC_SPRITE_H_
#define _SRC_SPRITE_H_

#include "devices/video/graphics.h"

typedef struct {
  uint32_t *colors;
  uint16_t width;
  uint16_t height;
  uint8_t is_clicked;
} Sprite_t;

Sprite_t *sprite_create(xpm_map_t sprite);

#endif
