#include <lcom/lcf.h>
#include <stdbool.h>
#include <stdint.h>
#include "sprite.h"
#include "i8042.h"
#include "video.h"


Sprite *create_sprite(xpm_image_t *pic, int x, int y, int xspeed, int yspeed) {
   Sprite *sp = (Sprite *) malloc ( sizeof(Sprite));
    sp->map = (uint32_t*)pic->bytes;
    sp->width = pic->width;
    sp->height = pic->height;
    sp->xi = x;
    sp->yi = y;
    sp->xf = x + sp->width;
    sp->yf = y + sp->height;
    sp->xspeed = xspeed;
    sp->yspeed = yspeed;
    return sp;
}

Food* create_food(xpm_image_t *pic, int x, int y, int xspeed, int yspeed, bool healthy) {
    Food *food = (Food *) malloc (sizeof(Food));
    Sprite *sp = (Sprite *) malloc ( sizeof(Sprite));
    sp->map = (uint32_t*)pic->bytes;
    sp->width = pic->width;
    sp->height = pic->height;
    sp->xi = x;
    sp->yi = y;
    sp->xf = sp->xi + sp->width;
    sp->yf = sp->yi + sp->height;
    sp->xspeed = xspeed;
    sp->yspeed = yspeed;
    food->sprite = sp;
    food->healthy = healthy;
    return food;
}

void destroy_sprite(Sprite *sprite) {
    if( sprite == NULL )
        return;
    /* Maps are borrowed from the global xpm_image_t tables loaded in
       load_xpms(); freeing one here invalidates every other sprite of the
       same fruit type. Only the per-sprite descriptor is owned. */
    free(sprite);
    sprite = NULL; // XXX: pointer is passed by value
    // should do this @ the caller
}

void destroy_food(Food *food) {
    if( food == NULL)
        return;
    destroy_sprite(food->sprite);
    free(food);
    food = NULL;
}

int draw_sprite(Sprite *sprite) {
    int width = sprite->width;
    int height = sprite->height;
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (sprite->map[i*width +j] != TRANSPARENCY_COLOR_8_8_8_8) 
                vg_color_pixel(sprite->xi + j, sprite->yi + i, sprite->map[i*width +j]);
        }
    }
    return 0;
}

int erase_sprite(Sprite *sprite, Sprite *background) {
    int width = sprite->width;
    int height = sprite->height;
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (sprite->map[i*width +j] != TRANSPARENCY_COLOR_8_8_8_8)
                vg_color_pixel(sprite->xi+j,sprite->yi+i,background->map[(sprite->yi+i)*background->width+j+sprite->xi]);
        }
    }
    return 0;
}
