#include "sprite.h"

sprite_t *sprite_create(xpm_map_t map) {
  sprite_t *sprite = (sprite_t *) malloc(sizeof(sprite_t));
  if (sprite == NULL)
    return NULL;

  sprite->map = (uint8_t *) xpm_load(map, XPM_8_8_8_8, &sprite->img);
  if (sprite->map == NULL) {
    free(sprite);
    return NULL;
  }

  sprite->x = 0;
  sprite->y = 0;

  return sprite;
}

void sprite_queue_destroy(void *sprite) {
  sprite_destroy((sprite_t *) sprite);
}

void sprite_destroy(sprite_t *sprite) {
  if (sprite == NULL)
    return;

  free(sprite->map);
}

int sprite_draw(sprite_t *sprite) {
  if (sprite == NULL)
    return 1;

  gph_t gph = vg_get_info();

  uint8_t *map = sprite->img.bytes;
  if (map == NULL)
    return 1;

  for (int j = 0; j < sprite->img.height; j++) {
    for (int i = 0; i < sprite->img.width; i++) {
      uint32_t color = 0;
      memcpy(&color, map, gph.bytes_per_pixel);

      if (color == xpm_transparency_color(sprite->img.type)) {
        map += gph.bytes_per_pixel;
        continue;
      }

      vg_draw_pixel(sprite->x + i, sprite->y + j, color);
      map += gph.bytes_per_pixel;
    }
  }
  return 0;
}

int sprite_draw_force_color(sprite_t *sprite, uint32_t color) {
  if (sprite == NULL)
    return 1;

  gph_t gph = vg_get_info();

  uint8_t *map = sprite->img.bytes;
  if (map == NULL)
    return 1;

  for (int j = 0; j < sprite->img.height; j++) {
    for (int i = 0; i < sprite->img.width; i++) {
      uint32_t temp_color = 0;
      memcpy(&temp_color, map, gph.bytes_per_pixel);

      if (temp_color == xpm_transparency_color(sprite->img.type)) {
        map += gph.bytes_per_pixel;
        continue;
      }

      vg_draw_pixel(sprite->x + i, sprite->y + j, color);
      map += gph.bytes_per_pixel;
    }
  }
  return 0;
}

int sprite_draw_rotate(sprite_t *sprite) {
  if (sprite == NULL)
    return 1;

  gph_t gph = vg_get_info();
  uint8_t *map = sprite->img.bytes;
  if (map == NULL)
    return 1;

  for (int j = 0; j < sprite->img.height; j++) {
    for (int i = 0; i < sprite->img.width; i++) {
      uint32_t color = 0;
      memcpy(&color, map, gph.bytes_per_pixel);

      if (color == xpm_transparency_color(sprite->img.type)) {
        map += gph.bytes_per_pixel;
        continue;
      }

      vg_draw_pixel(sprite->x + j, sprite->y + sprite->img.width - 1 - i, color);
      map += gph.bytes_per_pixel;
    }
  }

  return 0;
}

int sprite_move(sprite_t *sprite, uint32_t x, uint32_t y) {
  gph_t gph = vg_get_info();

  if (x >= gph.x_res || y >= gph.y_res)
    return 1;
  if (sprite == NULL)
    return 1;

  sprite->x = x;
  sprite->y = y;

  return 0;
}

int sprite_colides(sprite_t *sprite1, sprite_t *sprite2) {
  if (sprite1 == NULL || sprite2 == NULL)
    return 1;

  int x2_s1 = sprite1->x + sprite1->img.width;
  int y2_s1 = sprite1->y + sprite1->img.height;

  int x2_s2 = sprite2->x + sprite2->img.width;
  int y2_s2 = sprite2->y + sprite2->img.height;

  return x2_s1 > (int) sprite2->x && (int) sprite1->x < x2_s2 && y2_s1 > (int) sprite2->y && (int) sprite1->y < y2_s2;
  // s1_x2 > s2_x && s1_x < s2_x2 && s1_y2 > s2_y && s1_y < s2_y2
}
