#include "pvz_platform.h"
#include "sprite_loader.h"
#include "sprites.h"
#include "constants.h"

// --- Wall-nut Idle Frames (XPM source data) ---
#include "../../assets/plants/wall_nut/idle/wall_nut_idle_1.xpm"
#include "../../assets/plants/wall_nut/idle/wall_nut_idle_2.xpm"
#include "../../assets/plants/wall_nut/idle/wall_nut_idle_3.xpm"

static xpm_map_t wall_nut_idle_xpms[] = {
    (xpm_map_t) wall_nut_idle_1,
    (xpm_map_t) wall_nut_idle_2,
    (xpm_map_t) wall_nut_idle_3
};

static AnimSprite wall_nut_idle_sprite;

int load_wall_nut_sprites() {
    return sprite_load_frames(&wall_nut_idle_sprite, wall_nut_idle_xpms,
                              WALL_NUT_IDLE_FRAMES);
}

void free_wall_nut_sprites() {
    sprite_free_frames(&wall_nut_idle_sprite);
}

AnimSprite* get_wall_nut_idle_sprite() {
    return &wall_nut_idle_sprite;
}
