/**
 * @file sprite_utils.c
 * @brief Sprite utility helpers -- creation, solid fill, and scaled blitting.
 * @ingroup group_scene
 */

#include "sprite_utils.h"

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"
#include "video_gr_ext.h"

int sprite_utils_create(vg_sprite_t *sprite, uint16_t width, uint16_t height)
{
  if (sprite == NULL || width == 0 || height == 0)
    return 1;

  // calloc zeroes the buffer so the sprite starts fully transparent
  // (XPM_8_8_8_8 transparency colour is encoded as 0x00000000)
  uint8_t bytes_per_pixel = (uint8_t)get_bytes_per_pixel();
  size_t bytes            = (size_t)width * height * bytes_per_pixel;
  uint8_t *pixels          = calloc(bytes, 1);
  if (pixels == NULL)
    return 1;

  sprite->pixels          = pixels;
  sprite->width           = width;
  sprite->height          = height;
  sprite->bytes_per_pixel = bytes_per_pixel;
  return 0;
}

int sprite_utils_fill_rect(vg_sprite_t *sprite,
                           uint16_t x, uint16_t y,
                           uint16_t width, uint16_t height,
                           uint32_t color)
{
  if (sprite == NULL || sprite->pixels == NULL)
    return 1;
  if (x >= sprite->width || y >= sprite->height)
    return 0;

  width  = (uint16_t)util_clamp_i32((int32_t)width, 0, (int32_t)(sprite->width - x));
  height = (uint16_t)util_clamp_i32((int32_t)height, 0, (int32_t)(sprite->height - y));

  // Opaque the RGB color
  uint32_t encoded = color | 0xFF000000U;
  for (uint16_t row = 0; row < height; row++)
    for (uint16_t col = 0; col < width; col++) {
      uint8_t *dst = sprite->pixels +
                     ((size_t)(y + row) * sprite->width + x + col) * sprite->bytes_per_pixel;
      memcpy(dst, &encoded, sprite->bytes_per_pixel);
    }
  return 0;
}

int sprite_utils_draw_scaled(vg_sprite_t *dst, const vg_sprite_t *src,
                             uint16_t src_x, uint16_t src_y,
                             uint16_t width, uint16_t height,
                             uint16_t dst_x, uint16_t dst_y,
                             uint16_t scale)
{
  if (dst == NULL || src == NULL ||
      dst->pixels == NULL || src->pixels == NULL || scale == 0)
    return 1;

  if ((uint32_t)src_x + width > src->width || (uint32_t)src_y + height > src->height)
    return 1;

  const uint32_t transparent = xpm_transparency_color(XPM_8_8_8_8);
  for (uint16_t row = 0; row < height; row++)
    for (uint16_t col = 0; col < width; col++) {
      const uint8_t *sp = src->pixels +
                          ((size_t)(src_y + row) * src->width + src_x + col) *
                          src->bytes_per_pixel;
      uint32_t color = 0;
      // The copy to color is just to get the pixel value into a uint32_t for easy comparison with the transparency colour
      memcpy(&color, sp, sizeof(color));
      if (color == transparent)
        continue;

      // Scale each source pixel into a scale x scale block of pixels!
      for (uint16_t sr = 0; sr < scale; sr++)
        for (uint16_t sc = 0; sc < scale; sc++) {
          uint16_t dy = (uint16_t)(dst_y + row * scale + sr);
          uint16_t dx = (uint16_t)(dst_x + col * scale + sc);
          if (dy >= dst->height || dx >= dst->width)
            continue;
          uint8_t *dp = dst->pixels +
                        ((size_t)dy * dst->width + dx) * dst->bytes_per_pixel;
          memcpy(dp, sp, dst->bytes_per_pixel);
        }
    }
  return 0;
}
