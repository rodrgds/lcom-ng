#include "sprite.h"

struct Sprite {
    uint16_t width;
    uint16_t height;
    uint32_t *colors;  // NULL for solid sprites
};

uint16_t sprite_get_width(const Sprite *sprite) {
    if (sprite == NULL) return 0;
    return sprite->width;
}

uint16_t sprite_get_height(const Sprite *sprite) {
    if (sprite == NULL) return 0;
    return sprite->height;
}

const uint32_t* sprite_get_colors(const Sprite *sprite) {
    if (sprite == NULL) return NULL;
    return sprite->colors;
}

Sprite* create_sprite_xpm(xpm_map_t xpm) {
    Sprite *sp = (Sprite *) malloc(sizeof(Sprite));
    if (sp == NULL) return NULL;

    xpm_image_t img;
    sp->colors = (uint32_t *) xpm_load(xpm, GAME_XPM_TYPE, &img);

    if (sp->colors == NULL) {
        free(sp);
        return NULL;
    }

    sp->width   = img.width;
    sp->height  = img.height;

    return sp;
}

void destroy_sprite(Sprite *sprite) {
    if (sprite == NULL) return;
    if (sprite->colors != NULL) free(sprite->colors);
    free(sprite);
}

Sprite* create_sprite_flipped_horizontal(const Sprite *orig) {
    if (orig == NULL || orig->colors == NULL) return NULL;

    Sprite *sp = (Sprite *) malloc(sizeof(Sprite));
    if (sp == NULL) return NULL;

    sp->width = orig->width;
    sp->height = orig->height;

    unsigned int total_pixels = sp->width * sp->height;
    sp->colors = (uint32_t *) malloc(total_pixels * sizeof(uint32_t));
    if (sp->colors == NULL) {
        free(sp);
        return NULL;
    }

    for (int y = 0; y < sp->height; y++) {
        for (int x = 0; x < sp->width; x++) {
            int flipped_x = (sp->width - 1) - x;
            int orig_idx = y * sp->width + x;
            int new_idx = y * sp->width + flipped_x;
            sp->colors[new_idx] = orig->colors[orig_idx];
        }
    }

    return sp;
}
