#include "sprite.h"
#include <lcom/lcf.h>

extern bool changed;

Sprite *create_sprite(const char *pic[]) {
  // allocate space for the "object"
  Sprite *sp = (Sprite *) malloc(sizeof(Sprite));
  xpm_image_t img;
  if (sp == NULL)
    return NULL;

  sp->map = (uint32_t *) xpm_load(pic, XPM_8_8_8_8, &img);

  if (sp->map == NULL) {
    free(sp);
    return NULL;
  }

  sp->width = img.width;
  sp->height = img.height;

  // Initialize background fields
  sp->background = NULL;
  sp->last_x = -1;
  sp->last_y = -1;
  sp->has_background = false;

  return sp;
}

Sprite *create_sprite_neutral(const char *pic[]) {
  // allocate space for the "object"
  Sprite *sp = (Sprite *) malloc(sizeof(Sprite));
  xpm_image_t img;
  if (sp == NULL)
    return NULL;

  sp->map = (uint32_t *) xpm_load(pic, XPM_GRAY_8_8_8_8, &img);

  if (sp->map == NULL) {
    free(sp);
    return NULL;
  }

  sp->width = img.width;
  sp->height = img.height;
  sp->background = NULL;
  sp->last_x = -1;
  sp->last_y = -1;
  sp->has_background = false;

  return sp;
}

int draw_sprite(Sprite *sprite, int x, int y) {
  // Save current position
  sprite->last_x = x;
  sprite->last_y = y;

  // Free old background if exists
  if (sprite->background != NULL) {
    free(sprite->background);
    sprite->background = NULL;
  }

  // Allocate space for background
  // sprite->background = malloc(sprite->width * sprite->height * sizeof(uint32_t));
  // if (sprite->background == NULL) {
  //   printf("Failed to allocate background buffer\n");
  //   return 1;
  // }

  // // Save background pixels
  // for (int h = 0; h < sprite->height; h++) {
  //   for (int w = 0; w < sprite->width; w++) {
  //     uint32_t color;
  //     vg_read_pixel(x + w, y + h, &color);
  //     sprite->background[w + h * sprite->width] = color;
  //   }
  // }

  // sprite->has_background = true;

  // Now draw the sprite
  uint32_t color;
  for (int h = 0; h < sprite->height; h++) {
    for (int w = 0; w < sprite->width; w++) {
      color = sprite->map[w + h * sprite->width];
      if (color == xpm_transparency_color(XPM_8_8_8_8)) {
        continue;
      }
      if (vg_draw_pixel(x + w, y + h, color) != 0) {
        printf("Failed to draw pixel\n");
        return 1;
      }
    }
  }
  return 0;
}

int draw_sprite_and_restore(Sprite *sprite, int x, int y) {
  
  if( changed ) {
    changed = false;
  }else{
    // 1. Restaura a posição anterior (se aplicável)
    if (sprite->has_background && sprite->background != NULL) {
      for (int h = 0; h < sprite->height; h++) {
        for (int w = 0; w < sprite->width; w++) {
          uint32_t color = sprite->background[w + h * sprite->width];
          vg_draw_pixel(sprite->last_x + w, sprite->last_y + h, color);
        }
      }
      free(sprite->background);
      sprite->background = NULL;
      sprite->has_background = false;

    }
}
  // 2. Atualiza posição atual

  sprite->last_x = x;
  sprite->last_y = y;

  // Allocate space for background
  sprite->background = malloc(sprite->width * sprite->height * sizeof(uint32_t));
  if (sprite->background == NULL) {
    printf("Failed to allocate background buffer\n");
    return 1;
  }

  // Save background pixels
  for (int h = 0; h < sprite->height; h++) {
    for (int w = 0; w < sprite->width; w++) {

      uint32_t bg_color;
      if (vg_read_pixel(x + w, y + h, &bg_color) != 0) {
        printf("Failed to read background pixel at (%d, %d)\n", x + w, y + h);
        free(sprite->background);
        sprite->background = NULL;
        return 1;
      }
      sprite->background[w + h * sprite->width] = bg_color;
    }
  }

  
  sprite->has_background = true;

  for (int h = 0; h < sprite->height; h++) {
    for (int w = 0; w < sprite->width; w++) {
      uint32_t c = sprite->map[w + h * sprite->width];
      if (c == xpm_transparency_color(XPM_8_8_8_8)) {
        continue;
      }
      if (vg_draw_pixel(x + w, y + h, c) != 0) {
        printf("Failed to draw pixel\n");
        return 1;
      }
    }
  }
  return 0;
}

int restore_background(Sprite *sprite) {
  if (!sprite->has_background || sprite->background == NULL) {
    return 0; // Nothing to restore
  }

  for (int h = 0; h < sprite->height; h++) {
    for (int w = 0; w < sprite->width; w++) {
      uint32_t color = sprite->background[w + h * sprite->width];
      if (vg_draw_pixel(sprite->last_x + w,
                        sprite->last_y + h,
                        color) != 0) {
        printf("Failed to restore background pixel\n");
      }
    }
  }

  // Free background buffer
  free(sprite->background);
  sprite->background = NULL;
  sprite->has_background = false;

  return 0;
}

void destroy_sprite(Sprite *sp) {
  if (sp == NULL)
    return;
  if (sp->map)
    free(sp->map);
  if (sp->background)
    free(sp->background);
  free(sp);
}

Sprite *create_sprite_rectangle(int w, int h, uint32_t color) {
  // Allocate space for the sprite
  Sprite *sp = (Sprite *) malloc(sizeof(Sprite));
  if (sp == NULL) {
    return NULL;
  }

  // Set dimensions
  sp->width = w;
  sp->height = h;

  // Allocate memory for pixel data
  sp->map = malloc(w * h * sizeof(uint32_t));
  if (sp->map == NULL) {
    free(sp);
    return NULL;
  }

  // Fill the sprite with the specified color
  for (int i = 0; i < w * h; i++) {
    sp->map[i] = color;
  }

  // Initialize background fields
  sp->background = NULL;
  sp->last_x = -1;
  sp->last_y = -1;
  sp->has_background = false;

  return sp;
}
