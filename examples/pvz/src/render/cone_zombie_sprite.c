#include "pvz_platform.h"
#include <stdlib.h>
#include "sprite_loader.h"
#include "sprites.h"

#include "../../assets/zombies/conehead/walking/cone_walking_1.xpm"
#include "../../assets/zombies/conehead/walking/cone_walking_2.xpm"
#include "../../assets/zombies/conehead/walking/cone_walking_3.xpm"
#include "../../assets/zombies/conehead/walking/cone_walking_4.xpm"
#include "../../assets/zombies/conehead/walking/cone_walking_5.xpm"
#include "../../assets/zombies/conehead/walking/cone_walking_6.xpm"
#include "../../assets/zombies/conehead/walking/cone_walking_7.xpm"

#include "../../assets/zombies/conehead/eating/cone_eating_1.xpm"
#include "../../assets/zombies/conehead/eating/cone_eating_2.xpm"
#include "../../assets/zombies/conehead/eating/cone_eating_3.xpm"
#include "../../assets/zombies/conehead/eating/cone_eating_4.xpm"
#include "../../assets/zombies/conehead/eating/cone_eating_5.xpm"
#include "../../assets/zombies/conehead/eating/cone_eating_6.xpm"
#include "../../assets/zombies/conehead/eating/cone_eating_7.xpm"

static xpm_map_t conehead_walking_xpms[] = {
    (xpm_map_t) cone_walking_1,
    (xpm_map_t) cone_walking_2,
    (xpm_map_t) cone_walking_3,
    (xpm_map_t) cone_walking_4,
    (xpm_map_t) cone_walking_5,
    (xpm_map_t) cone_walking_6,
    (xpm_map_t) cone_walking_7
};

static xpm_map_t conehead_eating_xpms[] = {
    (xpm_map_t) cone_eating_1,
    (xpm_map_t) cone_eating_2,
    (xpm_map_t) cone_eating_3,
    (xpm_map_t) cone_eating_4,
    (xpm_map_t) cone_eating_5,
    (xpm_map_t) cone_eating_6,
    (xpm_map_t) cone_eating_7
};

static AnimSprite conehead_walking_sprite;
static AnimSprite conehead_eating_sprite;

static void free_anim_sprite(AnimSprite *sprite) {
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

static int load_anim_sprite(AnimSprite *sprite, xpm_map_t *xpms, int num_frames) {
    sprite->num_frames = num_frames;
    sprite->pixmaps = (uint8_t**) calloc(num_frames, sizeof(uint8_t*));
    if (sprite->pixmaps == NULL) return 1;

    for (int i = 0; i < num_frames; i++) {
        xpm_image_t img;
        sprite->pixmaps[i] = xpm_load(xpms[i], XPM_8_8_8, &img);
        if (sprite->pixmaps[i] == NULL) {
            free_anim_sprite(sprite);
            return 1;
        }

        if (i == 0) {
            sprite->width = img.width;
            sprite->height = img.height;
        }
    }

    return 0;
}

int load_cone_zombie_sprites(void) {
    if (load_anim_sprite(&conehead_walking_sprite, conehead_walking_xpms, CONEHEAD_WALKING_FRAMES) != 0) {
        return 1;
    }

    if (load_anim_sprite(&conehead_eating_sprite, conehead_eating_xpms, CONEHEAD_EATING_FRAMES) != 0) {
        free_anim_sprite(&conehead_walking_sprite);
        return 1;
    }

    return 0;
}

void free_cone_zombie_sprites(void) {
    free_anim_sprite(&conehead_walking_sprite);
    free_anim_sprite(&conehead_eating_sprite);
}

AnimSprite* get_conehead_walking_sprite(void) {
    return &conehead_walking_sprite;
}

AnimSprite* get_conehead_eating_sprite(void) {
    return &conehead_eating_sprite;
}
