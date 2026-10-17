#include "sprites.h"
#include "../devices/graphics/graphics.h"

sprite_t *(create_sprite)(xpm_map_t xpm) {
  // Allocate space for the "object"
  sprite_t *sp = (sprite_t *) malloc(sizeof(sprite_t));
  if (sp == NULL) {
    return NULL;
  }

  // read the sprite pixmap
  xpm_image_t img;
  sp->colors = xpm_load(xpm, XPM_INDEXED, &img);
  if (sp->colors == NULL) {
    free(sp);
    return NULL;
  }

  sp->width = img.width;
  sp->height = img.height;

  return sp;
}

void (destroy_sprite)(sprite_t *sp) {
  if (sp == NULL) {
    return;
  }

  if (sp->colors != NULL) {
    free(sp->colors);
  }

  free(sp);
  sp = NULL;
}

int (draw_sprite_color_override)(sprite_t *sp, uint16_t x, uint16_t y, uint8_t *color_override) {
  if (sp == NULL) {
    return 1;
  }

  for (int i = 0; i < sp->height; i++) {
    for (int j = 0; j < sp->width; j++) {
      uint32_t color = sp->colors[i * sp->width + j];

      if (color_override != NULL) {
        if (color == color_override[0]) {
          color = color_override[1];
        }
      }

      if (set_pixel_color(x + j, y + i, color)) {
        return 1;
      }
    }
  }

  return 0;
}

int (draw_sprite)(sprite_t *sp, uint16_t x, uint16_t y) {
  return draw_sprite_color_override(sp, x, y, NULL);
}
