#include "pvz_platform.h"
#include "sprite_loader.h"
#include "sprites.h"
#include "constants.h"

#include "../../assets/zombies/regular/walking/regular_walking_1.xpm"
#include "../../assets/zombies/regular/walking/regular_walking_2.xpm"
#include "../../assets/zombies/regular/walking/regular_walking_3.xpm"
#include "../../assets/zombies/regular/walking/regular_walking_4.xpm"
#include "../../assets/zombies/regular/walking/regular_walking_5.xpm"
#include "../../assets/zombies/regular/walking/regular_walking_6.xpm"
#include "../../assets/zombies/regular/walking/regular_walking_7.xpm"

static xpm_map_t regular_walking_xpms[] = {
    (xpm_map_t) regular_walking_1,
    (xpm_map_t) regular_walking_2,
    (xpm_map_t) regular_walking_3,
    (xpm_map_t) regular_walking_4,
    (xpm_map_t) regular_walking_5,
    (xpm_map_t) regular_walking_6,
    (xpm_map_t) regular_walking_7
};

static AnimSprite regular_walking_sprite;

int load_regular_walking_sprites() {
    return sprite_load_frames(&regular_walking_sprite, regular_walking_xpms,
                              REGULAR_WALKING_FRAMES);
}

void free_regular_walking_sprites() {
    sprite_free_frames(&regular_walking_sprite);
}

AnimSprite* get_regular_walking_sprite() {
    return &regular_walking_sprite;
}
