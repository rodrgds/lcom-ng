#include "pvz_platform.h"
#include "sprite_loader.h"
#include "sprites.h"
#include "constants.h"

// --- Repeater Idle Frames (XPM source data) ---
#include "../../assets/plants/repeater/idle/repeater_idle_1.xpm"
#include "../../assets/plants/repeater/idle/repeater_idle_2.xpm"
#include "../../assets/plants/repeater/idle/repeater_idle_3.xpm"
#include "../../assets/plants/repeater/idle/repeater_idle_4.xpm"
#include "../../assets/plants/repeater/idle/repeater_idle_5.xpm"

#include "../../assets/plants/repeater/shooting/repeater_shooting_1.xpm"
#include "../../assets/plants/repeater/shooting/repeater_shooting_2.xpm"

static xpm_map_t repeater_idle_xpms[] = {
    (xpm_map_t) repeater_idle_1,
    (xpm_map_t) repeater_idle_2,
    (xpm_map_t) repeater_idle_3,
    (xpm_map_t) repeater_idle_4,
    (xpm_map_t) repeater_idle_5
};

static xpm_map_t repeater_shooting_xpms[] = {
    (xpm_map_t) repeater_shooting_1,
    (xpm_map_t) repeater_shooting_2
};

static AnimSprite repeater_idle_sprite;
static AnimSprite repeater_shooting_sprite;
static uint16_t repeater_shooting_frame_widths[REPEATER_SHOOTING_FRAMES];
static uint16_t repeater_shooting_frame_heights[REPEATER_SHOOTING_FRAMES];

int load_repeater_sprites(void) {
    if (sprite_load_frames(&repeater_idle_sprite, repeater_idle_xpms,
                           REPEATER_IDLE_FRAMES) != 0) {
        return 1;
    }

    if (sprite_load_frames_with_dimensions(&repeater_shooting_sprite,
                                           repeater_shooting_xpms,
                                           repeater_shooting_frame_widths,
                                           repeater_shooting_frame_heights,
                                           REPEATER_SHOOTING_FRAMES) != 0) {
        free_repeater_sprites();
        return 1;
    }

    return 0;
}

void free_repeater_sprites(void) {
    sprite_free_frames(&repeater_idle_sprite);
    sprite_free_frames(&repeater_shooting_sprite);
}

AnimSprite* get_repeater_idle_sprite(void) {
    return &repeater_idle_sprite;
}

AnimSprite* get_repeater_shooting_sprite(void) {
    return &repeater_shooting_sprite;
}

uint16_t get_repeater_shooting_frame_width(int frame) {
    return sprite_frame_width(&repeater_shooting_sprite,
                              repeater_shooting_frame_widths,
                              frame);
}

uint16_t get_repeater_shooting_frame_height(int frame) {
    return sprite_frame_height(&repeater_shooting_sprite,
                               repeater_shooting_frame_heights,
                               frame);
}
