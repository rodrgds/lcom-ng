#include "pvz_platform.h"
#include "sprite_loader.h"
#include "sprites.h"
#include "constants.h"

#include "../../assets/zombies/regular/eating/reg_eating_1.xpm"
#include "../../assets/zombies/regular/eating/reg_eating_2.xpm"
#include "../../assets/zombies/regular/eating/reg_eating_3.xpm"
#include "../../assets/zombies/regular/eating/reg_eating_4.xpm"
#include "../../assets/zombies/regular/eating/reg_eating_5.xpm"
#include "../../assets/zombies/regular/eating/reg_eating_6.xpm"
#include "../../assets/zombies/regular/eating/reg_eating_7.xpm"

static xpm_map_t regular_eating_xpms[] = {
    (xpm_map_t) reg_eating_1,
    (xpm_map_t) reg_eating_2,
    (xpm_map_t) reg_eating_3,
    (xpm_map_t) reg_eating_4,
    (xpm_map_t) reg_eating_5,
    (xpm_map_t) reg_eating_6,
    (xpm_map_t) reg_eating_7
};

static AnimSprite regular_eating_sprite;

int load_regular_eating_sprites(void) {
    return sprite_load_frames(&regular_eating_sprite, regular_eating_xpms,
                              REGULAR_EATING_FRAMES);
}

void free_regular_eating_sprites(void) {
    sprite_free_frames(&regular_eating_sprite);
}

AnimSprite* get_regular_eating_sprite(void) {
    return &regular_eating_sprite;
}
