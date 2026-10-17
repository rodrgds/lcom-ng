#include "sprites.h"

Sprite* createSprite(xpm_map_t map) {
    Sprite* sprite = (Sprite*)malloc(sizeof(Sprite));
    if (sprite == NULL) {
        return NULL;
    }

    xpm_image_t image;
    sprite->bitmap = (uint32_t*)xpm_load(map, XPM_8_8_8_8, &image);
    sprite->height = image.height;
    sprite->width = image.width;

    if (sprite->bitmap == NULL) {
        free(sprite);
        return NULL;
    }

    return sprite;
}

void destroySprite(Sprite* sprite) {
    if (sprite == NULL) {
        return;
    }

    if (sprite->bitmap != NULL) {
        free(sprite->bitmap);
        sprite->bitmap = NULL;
    }

    free(sprite);
}
