#include "sprite.h"

#include <lcom/lcf.h>

Sprite* generate_Sprite(xpm_map_t xpm) {
    Sprite *s = (Sprite*) malloc (sizeof(Sprite));
    if (s == NULL) return NULL;
    xpm_image_t x;
    s->color_map = (uint32_t *) xpm_load(xpm, XPM_8_8_8_8, &x);
    s->h = x.height;
    s->w = x.width;

    if( s->color_map == NULL ) {
      return NULL;
    }
    return s;
}

void freeSprite(Sprite * sprite) {
    free(sprite->color_map);
    free(sprite);
    sprite=NULL;
}

void generate_all_sprites();
void freeAllSprites();
