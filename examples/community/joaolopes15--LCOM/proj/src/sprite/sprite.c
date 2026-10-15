#include "sprite.h"

Sprite *create_sprite(xpm_map_t xpm) {
    Sprite *sprite = (Sprite *)malloc(sizeof(Sprite));

    if (sprite == NULL) {
        return NULL;
    }

    xpm_image_t img;

    sprite->map = (uint32_t *) xpm_load(xpm, XPM_8_8_8_8, &img);

    if (sprite->map == NULL) {
        free(sprite);
        return NULL;
    }

    sprite->width = img.width;
    sprite->height = img.height;
    sprite->x = 0;
    sprite->y = 0;
    sprite->xspeed = 10;
    sprite->yspeed = 10;

    return sprite;
}

void destroy_sprite(Sprite *sprite) {
    if (sprite != NULL) {
        if (sprite->map != NULL) {
            free(sprite->map);
        }
        free(sprite);
    }
}

int draw_sprite(Sprite *sprite, uint16_t x, uint16_t y) {
  for (int i = 0; i < sprite->height; i++) {
    for (int j = 0; j < sprite->width; j++) {
      unsigned pixel_index = i * sprite->width + j;
      uint32_t color = sprite->map[pixel_index];

      if (color != xpm_transparency_color(XPM_8_8_8_8)) {
        if (vg_draw_pixel(x + j, y + i, color) != 0) {
          return 1;
        }
      }
    }
  }
  return 0;
}

int move_sprite_down(Sprite *sprite) {
    sprite->y += sprite->yspeed;

    if (sprite->y + sprite->height > vmi_p.YResolution) {
        sprite->y = vmi_p.YResolution - sprite->height;
        return 1;
    }

    return 0;
}

int move_sprite_up(Sprite *sprite) {
    sprite->y -= sprite->yspeed;

    if (sprite->y < 0) {
        sprite->y = 0;
        return 1;
    }

    return 0;
}

int move_sprite_left(Sprite *sprite) {
    sprite->x -= sprite->xspeed;

    if (sprite->x < 0) {
        sprite->x = 0;
        return 1;
    }

    return 0;
}

int move_sprite_right(Sprite *sprite) {
    sprite->x += sprite->xspeed;

    if (sprite->x + sprite->width > vmi_p.XResolution) {
        sprite->x = vmi_p.XResolution - sprite->width;
        return 1;
    }

    return 0;
}

void move_sprite_mouse(Sprite *sprite, int target_x, int target_y, int xspeed) {
    int dx = target_x - sprite->x;
    
    if (abs(dx) > xspeed) {
        sprite->x += (dx > 0) ? xspeed : -xspeed;
    } else {
        sprite->x = target_x;
    }

    if (sprite->x < 0) sprite->x = 0;
    if (sprite->x + sprite->width > vmi_p.XResolution) {
        sprite->x = vmi_p.XResolution - sprite->width;
    }
}
