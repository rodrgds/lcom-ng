#include "sprite.h"

/* Create and destroy section */

Sprite *create_sprite(xpm_map_t xpm, int16_t x, int16_t y, double xspeed, double yspeed) {
    Sprite *sp = (Sprite *) malloc(sizeof(Sprite));
    if (sp == NULL) {
        printf("create_sprite: malloc failed\n");
        return NULL;
    }

    xpm_image_t img;
    sp->map = xpm_load(xpm, XPM_INDEXED, &img);
    if (sp->map == NULL) {
        printf("create_sprite: xpm_load failed\n");
        free(sp);
        return NULL;
    }

    sp->x = x;
    sp->y = y;
    sp->width = img.width;
    sp->height = img.height;
    sp->xspeed = xspeed;
    sp->yspeed = yspeed;

    return sp;
}

int destroy_sprite(Sprite *sp) {
    if (sp == NULL) {
        printf("destroy_sprite: NULL pointer provided\n");
        return 1;
    }
    free(sp->map);
    free(sp);
    return 0;
}

/* Getter and setter section */

/* Statics section */

/* Others section */

/* Draw section */

int draw_sprite(Sprite *sprite, uint8_t *frame_buffer) {
    if (sprite == NULL) {
        printf("draw_sprite: NULL pointer provided\n");
        return 1;
    }

    if (frame_buffer == NULL) {
        printf("draw_sprite: NULL pointer pointer\n");
        return 1;
    }

    return vga_draw_loaded_xpm(sprite->map, sprite->x, sprite->y, sprite->width, sprite->height, frame_buffer);
}

int draw_transparent_sprite(Sprite *sprite, uint8_t *frame_buffer) {
    if (sprite == NULL) {
        printf("draw_transparent_sprite: NULL pointer provided\n");
        return 1;
    }

    if (frame_buffer == NULL) {
        printf("draw_transparent_sprite: NULL pointer provided\n");
        return 1;
    }

    if (sprite->x >= x_res || sprite->y >= y_res) {
        printf("draw_transparent_sprite: invalid sprite position, x: %d, y: %d\n", sprite->x, sprite->y);
        return 1;
    }

    uint8_t *map = sprite->map;

    for (int h = 0; h < sprite->height && sprite->y + h < y_res; h++) {
        for (int w = 0; w < sprite->width && sprite->x + w < x_res; w++, map++) {
            if (*map != BACKGROUND_COLOR) {
                vga_draw_pixel(sprite->x + w, sprite->y + h, *map, frame_buffer);
            }
        }
    }

    return 0;
}
