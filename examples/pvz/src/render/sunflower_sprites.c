#include "pvz_platform.h"
#include "sprite_loader.h"
#include "sprites.h"
#include "constants.h"

// --- Sunflower Idle Frames (XPM source data) ---
#include "../../assets/plants/sunflower/sunflower_idle_animation/sunflower_idle_1.xpm"
#include "../../assets/plants/sunflower/sunflower_idle_animation/sunflower_idle_2.xpm"
#include "../../assets/plants/sunflower/sunflower_idle_animation/sunflower_idle_3.xpm"
#include "../../assets/plants/sunflower/sunflower_idle_animation/sunflower_idle_4.xpm"
#include "../../assets/plants/sunflower/sunflower_idle_animation/sunflower_idle_5.xpm"
#include "../../assets/plants/sunflower/sunflower_idle_animation/sunflower_idle_6.xpm"

static xpm_map_t sunflower_idle_xpms[] = {
    (xpm_map_t) sunflower_idle_1,
    (xpm_map_t) sunflower_idle_2,
    (xpm_map_t) sunflower_idle_3,
    (xpm_map_t) sunflower_idle_4,
    (xpm_map_t) sunflower_idle_5,
    (xpm_map_t) sunflower_idle_6
};

static AnimSprite sunflower_idle_sprite;

int load_sunflower_sprites() {
    return sprite_load_frames(&sunflower_idle_sprite, sunflower_idle_xpms,
                              SUNFLOWER_IDLE_FRAMES);
}

void free_sunflower_sprites() {
    sprite_free_frames(&sunflower_idle_sprite);
}

AnimSprite* get_sunflower_idle_sprite() {
    return &sunflower_idle_sprite;
}
