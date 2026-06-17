#include "pvz_platform.h"
#include <stdlib.h>

#include "sprites.h"

#include "../../assets/zombies/pole_vaulting/walk/polewalking1.xpm"
#include "../../assets/zombies/pole_vaulting/walk/polewalking2.xpm"
#include "../../assets/zombies/pole_vaulting/walk/polewalking3.xpm"
#include "../../assets/zombies/pole_vaulting/walk/polewalking4.xpm"
#include "../../assets/zombies/pole_vaulting/walk/polewalking5.xpm"
#include "../../assets/zombies/pole_vaulting/walk/polewalking6.xpm"
#include "../../assets/zombies/pole_vaulting/walk/polewalkin8.xpm"

#include "../../assets/zombies/pole_vaulting/jump/jump1.xpm"
#include "../../assets/zombies/pole_vaulting/jump/jump2.xpm"
#include "../../assets/zombies/pole_vaulting/jump/jump3.xpm"
#include "../../assets/zombies/pole_vaulting/jump/jump4.xpm"
#include "../../assets/zombies/pole_vaulting/jump/jump5.xpm"
#include "../../assets/zombies/pole_vaulting/jump/jump6.xpm"

#include "../../assets/zombies/pole_vaulting/nopole/nopole1.xpm"
#include "../../assets/zombies/pole_vaulting/nopole/nopole2.xpm"
#include "../../assets/zombies/pole_vaulting/nopole/nopole3.xpm"
#include "../../assets/zombies/pole_vaulting/nopole/nopole4.xpm"
#include "../../assets/zombies/pole_vaulting/nopole/nopole5.xpm"
#include "../../assets/zombies/pole_vaulting/nopole/nopole6.xpm"
#include "../../assets/zombies/pole_vaulting/nopole/nopole7.xpm"

#include "../../assets/zombies/pole_vaulting/eating/eating2.xpm"
#include "../../assets/zombies/pole_vaulting/eating/eating3.xpm"
#include "../../assets/zombies/pole_vaulting/eating/eating4.xpm"
#include "../../assets/zombies/pole_vaulting/eating/eating5.xpm"
#include "../../assets/zombies/pole_vaulting/eating/eating6.xpm"
#include "../../assets/zombies/pole_vaulting/eating/eating7.xpm"

static xpm_map_t pole_vaulting_walking_xpms[] = {
    (xpm_map_t) polewalking1_xpm,
    (xpm_map_t) polewalking2_xpm,
    (xpm_map_t) polewalking3_xpm,
    (xpm_map_t) polewalking4_xpm,
    (xpm_map_t) polewalking5_xpm,
    (xpm_map_t) polewalking6_xpm,
    (xpm_map_t) polewalkin8_xpm
};

static xpm_map_t pole_vaulting_jump_xpms[] = {
    (xpm_map_t) jump1_xpm,
    (xpm_map_t) jump2_xpm,
    (xpm_map_t) jump3_xpm,
    (xpm_map_t) jump4_xpm,
    (xpm_map_t) jump5_xpm,
    (xpm_map_t) jump6_xpm
};

static xpm_map_t pole_vaulting_nopole_xpms[] = {
    (xpm_map_t) nopole1_xpm,
    (xpm_map_t) nopole2_xpm,
    (xpm_map_t) nopole3_xpm,
    (xpm_map_t) nopole4_xpm,
    (xpm_map_t) nopole5_xpm,
    (xpm_map_t) nopole6_xpm,
    (xpm_map_t) nopole7_xpm
};

static xpm_map_t pole_vaulting_eating_xpms[] = {
    (xpm_map_t) nopole1_xpm,
    (xpm_map_t) eating2_xpm,
    (xpm_map_t) eating3_xpm,
    (xpm_map_t) eating4_xpm,
    (xpm_map_t) eating5_xpm,
    (xpm_map_t) eating6_xpm,
    (xpm_map_t) eating7_xpm
};

static AnimSprite pole_vaulting_walking_sprite;
static AnimSprite pole_vaulting_jump_sprite;
static AnimSprite pole_vaulting_nopole_sprite;
static AnimSprite pole_vaulting_eating_sprite;
static uint16_t pole_vaulting_walking_frame_widths[POLE_VAULTING_WALKING_FRAMES];
static uint16_t pole_vaulting_walking_frame_heights[POLE_VAULTING_WALKING_FRAMES];
static uint16_t pole_vaulting_jump_frame_widths[POLE_VAULTING_JUMP_FRAMES];
static uint16_t pole_vaulting_jump_frame_heights[POLE_VAULTING_JUMP_FRAMES];
static uint16_t pole_vaulting_nopole_frame_widths[POLE_VAULTING_NOPOLE_FRAMES];
static uint16_t pole_vaulting_nopole_frame_heights[POLE_VAULTING_NOPOLE_FRAMES];
static uint16_t pole_vaulting_eating_frame_widths[POLE_VAULTING_EATING_FRAMES];
static uint16_t pole_vaulting_eating_frame_heights[POLE_VAULTING_EATING_FRAMES];

static void free_variable_anim_sprite(AnimSprite *sprite) {
    if (sprite->pixmaps != NULL) {
        for (int i = 0; i < sprite->num_frames; i++) {
            if (sprite->pixmaps[i] != NULL) {
                free(sprite->pixmaps[i]);
            }
        }

        free(sprite->pixmaps);
        sprite->pixmaps = NULL;
    }

    sprite->num_frames = 0;
}

static int load_variable_anim_sprite(AnimSprite *sprite, xpm_map_t *xpms,
                                     uint16_t *widths, uint16_t *heights,
                                     int num_frames) {
    sprite->num_frames = num_frames;
    sprite->pixmaps = (uint8_t **) calloc(num_frames, sizeof(uint8_t *));
    if (sprite->pixmaps == NULL) return 1;

    for (int i = 0; i < num_frames; i++) {
        xpm_image_t img;

        sprite->pixmaps[i] = xpm_load(xpms[i], XPM_8_8_8, &img);
        if (sprite->pixmaps[i] == NULL) {
            free_variable_anim_sprite(sprite);
            return 1;
        }

        widths[i] = img.width;
        heights[i] = img.height;
        if (i == 0) {
            sprite->width = img.width;
            sprite->height = img.height;
        }
    }

    return 0;
}

int load_pole_vaulting_zombie_sprites(void) {
    if (load_variable_anim_sprite(&pole_vaulting_walking_sprite,
                                  pole_vaulting_walking_xpms,
                                  pole_vaulting_walking_frame_widths,
                                  pole_vaulting_walking_frame_heights,
                                  POLE_VAULTING_WALKING_FRAMES) != 0) {
        return 1;
    }

    if (load_variable_anim_sprite(&pole_vaulting_jump_sprite,
                                  pole_vaulting_jump_xpms,
                                  pole_vaulting_jump_frame_widths,
                                  pole_vaulting_jump_frame_heights,
                                  POLE_VAULTING_JUMP_FRAMES) != 0) {
        free_variable_anim_sprite(&pole_vaulting_walking_sprite);
        return 1;
    }

    if (load_variable_anim_sprite(&pole_vaulting_nopole_sprite,
                                  pole_vaulting_nopole_xpms,
                                  pole_vaulting_nopole_frame_widths,
                                  pole_vaulting_nopole_frame_heights,
                                  POLE_VAULTING_NOPOLE_FRAMES) != 0) {
        free_variable_anim_sprite(&pole_vaulting_walking_sprite);
        free_variable_anim_sprite(&pole_vaulting_jump_sprite);
        return 1;
    }

    if (load_variable_anim_sprite(&pole_vaulting_eating_sprite,
                                  pole_vaulting_eating_xpms,
                                  pole_vaulting_eating_frame_widths,
                                  pole_vaulting_eating_frame_heights,
                                  POLE_VAULTING_EATING_FRAMES) != 0) {
        free_variable_anim_sprite(&pole_vaulting_walking_sprite);
        free_variable_anim_sprite(&pole_vaulting_jump_sprite);
        free_variable_anim_sprite(&pole_vaulting_nopole_sprite);
        return 1;
    }

    return 0;
}

void free_pole_vaulting_zombie_sprites(void) {
    free_variable_anim_sprite(&pole_vaulting_walking_sprite);
    free_variable_anim_sprite(&pole_vaulting_jump_sprite);
    free_variable_anim_sprite(&pole_vaulting_nopole_sprite);
    free_variable_anim_sprite(&pole_vaulting_eating_sprite);
}

AnimSprite *get_pole_vaulting_walking_sprite(void) {
    return &pole_vaulting_walking_sprite;
}

AnimSprite *get_pole_vaulting_jump_sprite(void) {
    return &pole_vaulting_jump_sprite;
}

AnimSprite *get_pole_vaulting_nopole_sprite(void) {
    return &pole_vaulting_nopole_sprite;
}

AnimSprite *get_pole_vaulting_eating_sprite(void) {
    return &pole_vaulting_eating_sprite;
}

uint16_t get_pole_vaulting_walking_frame_width(int frame) {
    return pole_vaulting_walking_frame_widths[frame];
}

uint16_t get_pole_vaulting_walking_frame_height(int frame) {
    return pole_vaulting_walking_frame_heights[frame];
}

uint16_t get_pole_vaulting_jump_frame_width(int frame) {
    return pole_vaulting_jump_frame_widths[frame];
}

uint16_t get_pole_vaulting_jump_frame_height(int frame) {
    return pole_vaulting_jump_frame_heights[frame];
}

uint16_t get_pole_vaulting_nopole_frame_width(int frame) {
    return pole_vaulting_nopole_frame_widths[frame];
}

uint16_t get_pole_vaulting_nopole_frame_height(int frame) {
    return pole_vaulting_nopole_frame_heights[frame];
}

uint16_t get_pole_vaulting_eating_frame_width(int frame) {
    return pole_vaulting_eating_frame_widths[frame];
}

uint16_t get_pole_vaulting_eating_frame_height(int frame) {
    return pole_vaulting_eating_frame_heights[frame];
}
