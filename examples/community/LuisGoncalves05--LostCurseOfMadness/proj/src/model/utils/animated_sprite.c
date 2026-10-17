#include "animated_sprite.h"

/* Create and destroy section */

AnimatedSprite *create_animated_sprite(Sprite *sprite, uint32_t frames_per_sprite, uint32_t number_sprites, ...) {
    AnimatedSprite *anim_sprite = malloc(sizeof(AnimatedSprite));
    if (anim_sprite == NULL) {
        printf("create_animated_sprite: malloc failed\n");
        return NULL;
    }

    anim_sprite->sprite = sprite;
    anim_sprite->frames_per_sprite = frames_per_sprite;
    anim_sprite->current_frame = 0;
    anim_sprite->number_sprites = number_sprites;
    anim_sprite->current_sprite = 0;

    anim_sprite->maps = malloc(number_sprites * sizeof(uint8_t *));
    if (anim_sprite->maps == NULL) {
        printf("create_animated_sprite: malloc failed\n");
        free(anim_sprite);
        return NULL;
    }
    anim_sprite->maps[0] = sprite->map;

    va_list args_pointer;
    va_start(args_pointer, number_sprites);
    for (int i = 1; i < (int) number_sprites; i++) {
        xpm_map_t tpm = va_arg(args_pointer, xpm_map_t);
        xpm_image_t img;
        anim_sprite->maps[i] = xpm_load(tpm, XPM_INDEXED, &img);
        if (anim_sprite->maps[i] == NULL) {
            printf("create_animated_sprite: xpm_load failed\n");
            for (int j = 0; j < i; j++) {
                free(anim_sprite->maps[j]);
            }
            free(anim_sprite->maps);
            free(anim_sprite);
            va_end(args_pointer);
            return NULL;
        }
    }
    va_end(args_pointer);

    return anim_sprite;
}

int destroy_animated_sprite(AnimatedSprite *anim_sprite) {
    if (anim_sprite == NULL) {
        printf("destroy_animated_sprite: NULL pointer provided\n");
        return 1;
    }

    free(anim_sprite->sprite);
    for (int i = 0; i < (int) anim_sprite->number_sprites; i++) {
        free(anim_sprite->maps[i]);
    }

    free(anim_sprite->maps);
    free(anim_sprite);

    return 0;
}

/* Getter and setter section */

/* Statics section */

/* Others section */

/* Draw section */

int draw_animated_sprite(AnimatedSprite *sprite, uint8_t *frame_buffer) {
    if (sprite == NULL) {
        printf("draw_animated_sprite: NULL pointer provided.\n");
        return 1;
    }

    if (sprite->current_sprite >= sprite->number_sprites) {
        printf("draw_animated_sprite: current sprite index out of bounds.\n");
        return 1;
    }

    Sprite *current_sprite = sprite->sprite;
    current_sprite->map = sprite->maps[sprite->current_sprite];

    if (draw_transparent_sprite(current_sprite, frame_buffer) != 0) {
        printf("draw_animated_sprite: draw_transparent_sprite failed.\n");
        return 1;
    }

    sprite->current_frame++;
    if (sprite->current_frame >= sprite->frames_per_sprite) {
        sprite->current_frame = 0;
        sprite->current_sprite = (sprite->current_sprite + 1) % sprite->number_sprites;
    }

    return 0;
}
