#include "../include/game/sprite.h"

int sprite_load(sprite_t *sprite, xpm_map_t xpm) {
    if (sprite == NULL) return 1;

    xpm_image_t img;
    uint8_t *pixmap = xpm_load(xpm, XPM_8_8_8, &img);
    if (pixmap == NULL) {
        printf("[sprite] failed to load xpm\n");
        return 1;
    }

    sprite->pixmap = pixmap;
    sprite->width = img.width;
    sprite->height = img.height;

    return 0;
}

void sprite_destroy(sprite_t *sprite) {
    if (sprite != NULL && sprite->pixmap != NULL) {
        free(sprite->pixmap);
        sprite->pixmap = NULL;
    }
}

int anim_sprite_load(anim_sprite_t *anim, sprite_t *sprites[], size_t count,
                     xpm_map_t xpms[], int frame_delay) {
    if (anim == NULL || sprites == NULL || xpms == NULL || count == 0) return 1;

    for (size_t i = 0; i < count; i++) {
        if (sprite_load(sprites[i], xpms[i]) != 0) {
            printf("[anim] failed to load frame %u\n", (unsigned)i);
            for (size_t j = 0; j < i; j++) {
                sprite_destroy(sprites[j]);
            }
            return 1;
        }
    }

    anim->sprites = sprites;
    anim->num_sprites = count;
    anim->cur_frame = 0;
    anim->last_tick = 0;
    anim->frame_delay = frame_delay;

    return 0;
}
