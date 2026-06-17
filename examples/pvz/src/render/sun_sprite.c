#include "pvz_platform.h"
#include "sprite_loader.h"
#include "sprites.h"
#include "constants.h"

#include "../../assets/ui/sun/sun_1.xpm"
#include "../../assets/ui/sun/sun_2.xpm"

static xpm_map_t sun_xpms[] = {
    (xpm_map_t) sun_1,
    (xpm_map_t) sun_2
};

static AnimSprite sun_sprite;

int load_sun_sprite(void) {
    return sprite_load_frames(&sun_sprite, sun_xpms, SUN_FRAMES);
}

void free_sun_sprite(void) {
    sprite_free_frames(&sun_sprite);
}

AnimSprite* get_sun_sprite(void) {
    return &sun_sprite;
}
