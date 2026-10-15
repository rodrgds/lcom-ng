#include <lcom/lcf.h>

#include "model/sprite/sprite.h"

Sprite *(create_sprite) (const char *pic[]) {
  Sprite *sp = (Sprite *) malloc(sizeof(Sprite));
  if (sp == NULL) {
    fprintf(stderr, "create_sprite: failed to allocate memory for sprite.");
    return NULL;
  }

  xpm_image_t img;
  sp->map = (char *) xpm_load(pic, XPM_8_8_8_8, &img);
  if (sp->map == NULL) {
    fprintf(stderr, "create_sprite: failed to load XPM image.");
    free(sp);
    return NULL;
  }

  sp->width = img.width;
  sp->height = img.height;
  return sp;
}

void(destroy_sprite)(Sprite *sp) {
  if (sp == NULL) {
    fprintf(stderr, "destroy_sprite: sp pointer cannot be null.");
    return;
  }
  if (sp->map) {
    free(sp->map);
  }
  free(sp);
  sp = NULL;
}

int(draw_sprite)(Sprite *sp, int16_t x, int16_t y) {
  if (sp == NULL) {
    fprintf(stderr, "draw_sprite: sp pointer cannot be null.");
    return 1;
  }
  if (x >= vbe_get_h_res() || y >= vbe_get_v_res()) {
    fprintf(stderr, "draw_sprite: invalid coordinates.");
    return 1;
  }
  if (x + sp->width > vbe_get_h_res() || y + sp->height > vbe_get_v_res()) {
    fprintf(stderr, "draw_sprite: invalid dimensions.");
    return 1;
  }

  uint32_t color;
  for (uint16_t row = 0; row < sp->height; row++) {
    for (uint16_t col = 0; col < sp->width; col++) {
      color = *((uint32_t *) (&sp->map[(row * sp->width + col) * 4]));

      if (color == xpm_transparency_color(XPM_8_8_8_8)) {
        continue;
      }

      if (graphics_draw_pixel(x + col, y + row, color) != 0) {
        fprintf(stderr, "draw_sprite: failed to draw pixel.");
        return 1;
      }
    }
  }

  return 0;
}

int(draw_sprite_region)(Sprite *sp, int16_t dest_x, int16_t dest_y, uint16_t src_x, uint16_t src_y, uint16_t width, uint16_t height) {
  if (sp == NULL) {
    fprintf(stderr, "draw_sprite_region: sp pointer cannot be null.");
    return 1;
  }
  if (dest_x >= vbe_get_h_res() || dest_y >= vbe_get_v_res()) {
    fprintf(stderr, "draw_sprite_region: invalid destination coordinates.");
    return 1;
  }
  if (src_x >= sp->width || src_y >= sp->height) {
    fprintf(stderr, "draw_sprite_region: invalid source coordinates.");
    return 1;
  }
  if (src_x + width > sp->width || src_y + height > sp->height) {
    fprintf(stderr, "draw_sprite_region: region exceeds sprite bounds.");
    return 1;
  }
  if (dest_x + width > vbe_get_h_res() || dest_y + height > vbe_get_v_res()) {
    fprintf(stderr, "draw_sprite_region: region exceeds screen bounds.");
    return 1;
  }

  uint32_t color;
  for (uint16_t row = 0; row < height; row++) {
    for (uint16_t col = 0; col < width; col++) {
      uint16_t sprite_row = src_y + row;
      uint16_t sprite_col = src_x + col;
      color = *((uint32_t *) (&sp->map[(sprite_row * sp->width + sprite_col) * 4]));

      if (color == xpm_transparency_color(XPM_8_8_8_8)) {
        continue;
      }

      if (graphics_draw_pixel(dest_x + col, dest_y + row, color) != 0) {
        fprintf(stderr, "draw_sprite_region: failed to draw pixel.");
        return 1;
      }
    }
  }

  return 0;
}
