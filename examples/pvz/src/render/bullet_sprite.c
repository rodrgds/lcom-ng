#include "pvz_platform.h"
#include "sprite_loader.h"
#include "sprites.h"
#include "constants.h"

#include "../../assets/projectile/pea/pea1.xpm"
#include "../../assets/projectile/pea/pea2.xpm"
#include "../../assets/projectile/pea/pea3.xpm"

static xpm_map_t pea_xpms[] = {
    (xpm_map_t) pea1,
    (xpm_map_t) pea2,
    (xpm_map_t) pea3
};

static AnimSprite bullet_sprite;
static uint16_t bullet_frame_widths[PEA_FRAMES];
static uint16_t bullet_frame_heights[PEA_FRAMES];

int load_bullet_sprite() {
    return sprite_load_frames_with_dimensions(&bullet_sprite, pea_xpms,
                                              bullet_frame_widths,
                                              bullet_frame_heights,
                                              PEA_FRAMES);
}

void free_bullet_sprite() {
    sprite_free_frames(&bullet_sprite);
}

AnimSprite* get_bullet_sprite() {
    return &bullet_sprite;
}

uint16_t get_bullet_frame_width(int frame) {
    return sprite_frame_width(&bullet_sprite, bullet_frame_widths, frame);
}

uint16_t get_bullet_frame_height(int frame) {
    return sprite_frame_height(&bullet_sprite, bullet_frame_heights, frame);
}
