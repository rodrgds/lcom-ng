#include "pvz_platform.h"
#include <stdlib.h>

#include "sprite_loader.h"

void sprite_free_frames(AnimSprite *sprite) {
    if (sprite == NULL) {
        return;
    }

    if (sprite->pixmaps != NULL) {
        for (int i = 0; i < sprite->num_frames; i++) {
            free(sprite->pixmaps[i]);
        }

        free(sprite->pixmaps);
        sprite->pixmaps = NULL;
    }

    sprite->num_frames = 0;
    sprite->width = 0;
    sprite->height = 0;
}

int sprite_load_frames_with_dimensions(AnimSprite *sprite,
                                       xpm_map_t *xpms,
                                       uint16_t *widths,
                                       uint16_t *heights,
                                       int num_frames) {
    if (sprite == NULL || xpms == NULL || num_frames <= 0) {
        return 1;
    }

    sprite->num_frames = num_frames;
    sprite->pixmaps = (uint8_t**) calloc(num_frames, sizeof(uint8_t*));
    if (sprite->pixmaps == NULL) {
        sprite->num_frames = 0;
        return 1;
    }

    for (int i = 0; i < num_frames; i++) {
        xpm_image_t img;

        sprite->pixmaps[i] = xpm_load(xpms[i], XPM_8_8_8, &img);
        if (sprite->pixmaps[i] == NULL) {
            sprite_free_frames(sprite);
            return 1;
        }

        if (widths != NULL) {
            widths[i] = img.width;
        }
        if (heights != NULL) {
            heights[i] = img.height;
        }

        if (i == 0) {
            sprite->width = img.width;
            sprite->height = img.height;
        }
    }

    return 0;
}

int sprite_load_frames(AnimSprite *sprite, xpm_map_t *xpms, int num_frames) {
    return sprite_load_frames_with_dimensions(sprite, xpms, NULL, NULL, num_frames);
}

int sprite_load_single(AnimSprite *sprite, xpm_map_t xpm) {
    xpm_map_t frames[] = {xpm};

    return sprite_load_frames(sprite, frames, 1);
}

uint16_t sprite_frame_width(const AnimSprite *sprite, const uint16_t *widths, int frame) {
    if (sprite == NULL || sprite->num_frames == 0) {
        return 0;
    }

    if (widths == NULL) {
        return sprite->width;
    }

    return widths[frame % sprite->num_frames];
}

uint16_t sprite_frame_height(const AnimSprite *sprite, const uint16_t *heights, int frame) {
    if (sprite == NULL || sprite->num_frames == 0) {
        return 0;
    }

    if (heights == NULL) {
        return sprite->height;
    }

    return heights[frame % sprite->num_frames];
}
