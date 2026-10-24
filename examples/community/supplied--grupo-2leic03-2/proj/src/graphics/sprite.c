#include "sprite.h"

#include <stdlib.h>
#include <string.h>

#include "graphics.h"

static enum xpm_image_type sprite_get_xpm_type(void) {
  uint8_t bpp = graphics_get_bytes_per_pixel();

  if (bpp == 1) {
    return XPM_INDEXED;
  }

  if (bpp == 2) {
    return XPM_5_6_5;
  }

  if (bpp == 3) {
    return XPM_8_8_8;
  }

  return XPM_8_8_8_8;
}

static uint32_t sprite_read_color(const Sprite *sprite, uint32_t index) {
  uint32_t color = 0;

  memcpy(&color,
         sprite->map + index * sprite->bytes_per_pixel,
         sprite->bytes_per_pixel);

  return color;
}

int sprite_load(Sprite *sprite, xpm_map_t xpm) {
  if (sprite == NULL || xpm == NULL) {
    return 1;
  }

  memset(sprite, 0, sizeof(Sprite));

  xpm_image_t image;
  enum xpm_image_type type = sprite_get_xpm_type();

  uint8_t *map = xpm_load(xpm, type, &image);

  if (map == NULL) {
    return 1;
  }

  sprite->width = image.width;
  sprite->height = image.height;
  sprite->bytes_per_pixel = graphics_get_bytes_per_pixel();
  sprite->transparent_color = xpm_transparency_color(type);
  sprite->map = map;
  sprite->loaded = true;

  return 0;
}

void sprite_draw(const Sprite *sprite, int x, int y) {
  if (sprite == NULL || !sprite->loaded || sprite->map == NULL) return;

  int screen_width = graphics_get_width();
  int screen_height = graphics_get_height();

  for (uint16_t row = 0; row < sprite->height; row++) {
    int screen_y = y + row;

    if (screen_y < 0 || screen_y >= screen_height) continue;

    for (uint16_t col = 0; col < sprite->width; col++) {
      int screen_x = x + col;

      if (screen_x < 0 || screen_x >= screen_width) continue;

      uint32_t index = row * sprite->width + col;
      uint32_t color = sprite_read_color(sprite, index);
      if (color == sprite->transparent_color) continue;

      graphics_draw_pixel((uint16_t) screen_x, (uint16_t) screen_y, color);
    }
  }
}

void sprite_draw_scaled(const Sprite *sprite, int x, int y, uint8_t scale) {
  if (sprite == NULL || !sprite->loaded || sprite->map == NULL || scale == 0) return;

  if (scale == 1) {
    sprite_draw(sprite, x, y);
    return;
  }

  int screen_width = graphics_get_width();
  int screen_height = graphics_get_height();

  for (uint16_t row = 0; row < sprite->height; row++) {
    for (uint16_t col = 0; col < sprite->width; col++) {
      uint32_t index = row * sprite->width + col;
      uint32_t color = sprite_read_color(sprite, index);

      if (color == sprite->transparent_color) continue;

      int screen_x = x + col * scale;
      int screen_y = y + row * scale;

      if (screen_x >= screen_width || screen_y >= screen_height) continue;
      if (screen_x + scale <= 0 || screen_y + scale <= 0) continue;

      graphics_draw_rectangle_clipped(screen_x, screen_y, scale, scale, color);
    }
  }
}

void sprite_draw_centered(const Sprite *sprite, int center_x, int center_y) {
  if (sprite == NULL || !sprite->loaded || sprite->map == NULL) {
    return;
  }

  int x = center_x - sprite->width / 2;
  int y = center_y - sprite->height / 2;

  sprite_draw(sprite, x, y);
}

void sprite_draw_scaled_centered(const Sprite *sprite, int center_x, int center_y, uint8_t scale) {
  if (sprite == NULL || !sprite->loaded || sprite->map == NULL || scale == 0) {
    return;
  }

  int draw_width = sprite->width * scale;
  int draw_height = sprite->height * scale;

  sprite_draw_scaled(sprite,
                     center_x - draw_width / 2,
                     center_y - draw_height / 2,
                     scale);
}

void sprite_destroy(Sprite *sprite) {
  if (sprite == NULL) {
    return;
  }

  if (sprite->map != NULL) {
    free(sprite->map);
    sprite->map = NULL;
  }

  sprite->width = 0;
  sprite->height = 0;
  sprite->bytes_per_pixel = 0;
  sprite->transparent_color = 0;
  sprite->loaded = false;
}
