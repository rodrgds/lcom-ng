#include "pvz_platform.h"
#include "sprite_loader.h"
#include "sprites.h"
#include "constants.h"

#include "../../assets/projectile/cabbage/cabbage_1.xpm"
#include "../../assets/projectile/cabbage/cabbage_2.xpm"
#include "../../assets/projectile/cabbage/cabbage_3.xpm"
#include "../../assets/projectile/cabbage/cabbage_4.xpm"

static xpm_map_t cabbage_projectile_xpms[] = {
    (xpm_map_t) cabbage_1_xpm,
    (xpm_map_t) cabbage_2_xpm,
    (xpm_map_t) cabbage_3_xpm,
    (xpm_map_t) cabbage_4_xpm
};

static AnimSprite cabbage_projectile_sprite;
static uint16_t cabbage_projectile_frame_widths[CABBAGE_PROJECTILE_FRAMES];
static uint16_t cabbage_projectile_frame_heights[CABBAGE_PROJECTILE_FRAMES];

int load_cabbage_projectile_sprite(void) {
    return sprite_load_frames_with_dimensions(&cabbage_projectile_sprite,
                                              cabbage_projectile_xpms,
                                              cabbage_projectile_frame_widths,
                                              cabbage_projectile_frame_heights,
                                              CABBAGE_PROJECTILE_FRAMES);
}

void free_cabbage_projectile_sprite(void) {
    sprite_free_frames(&cabbage_projectile_sprite);
}

AnimSprite* get_cabbage_projectile_sprite(void) {
    return &cabbage_projectile_sprite;
}

uint16_t get_cabbage_projectile_frame_width(int frame) {
    return sprite_frame_width(&cabbage_projectile_sprite, cabbage_projectile_frame_widths, frame);
}

uint16_t get_cabbage_projectile_frame_height(int frame) {
    return sprite_frame_height(&cabbage_projectile_sprite, cabbage_projectile_frame_heights, frame);
}
