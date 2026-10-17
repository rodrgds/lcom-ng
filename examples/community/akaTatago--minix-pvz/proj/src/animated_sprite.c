#include "animated_sprite.h"

int init_animated_sprite(struct animated_sprite *sprite, uint8_t num_frames, uint8_t ticks_per_frame, 
    char **xpm_maps[], enum xpm_image_type type) {
    if (num_frames > MAX_FRAMES_PER_SPRITE || sprite == NULL || xpm_maps == NULL) {
        return 1; 
    }

    sprite->frames = num_frames;
    sprite->current_frame = 0;

    sprite->ticks_per_frame = ticks_per_frame;
    sprite->tick_counter = 0;

    for (uint8_t i = 0; i < num_frames; i++) {
        
        uint8_t *pixmap = xpm_load((xpm_map_t) xpm_maps[i], type, &(sprite->imgs[i]));
        
        if (pixmap == NULL) {
            return 1; 
        }

        sprite->pixmaps[i] = pixmap;
    }

    return 0;
}

xpm_image_t get_animated_sprite_image(const struct animated_sprite* sprite) {
    return sprite->imgs[sprite->current_frame];
}

uint8_t* get_animated_sprite_pixmap(const struct animated_sprite* sprite) {
    return sprite->pixmaps[sprite->current_frame];
}

void tick_animated_sprite(struct animated_sprite* sprite) {
    sprite->tick_counter++;

    if (sprite->tick_counter >= sprite->ticks_per_frame) {
        sprite->tick_counter = 0;
        sprite->current_frame = (sprite->current_frame + 1) % sprite->frames;
    }
}
