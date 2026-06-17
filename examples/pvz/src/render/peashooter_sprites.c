#include "pvz_platform.h"
#include "sprite_loader.h"
#include "sprites.h"
#include "constants.h"

// --- Peashooter Idle Frames (XPM source data) ---
#include "../../assets/plants/peashooter/peashooter_idle_animation/peashooter_idle_1.xpm"
#include "../../assets/plants/peashooter/peashooter_idle_animation/peashooter_idle_2.xpm"
#include "../../assets/plants/peashooter/peashooter_idle_animation/peashooter_idle_3.xpm"
#include "../../assets/plants/peashooter/peashooter_idle_animation/peashooter_idle_4.xpm"
#include "../../assets/plants/peashooter/peashooter_idle_animation/peashooter_idle_5.xpm"
#include "../../assets/plants/peashooter/peashooter_idle_animation/peashooter_idle_6.xpm"
#include "../../assets/plants/peashooter/peashooter_idle_animation/peashooter_idle_7.xpm"
#include "../../assets/plants/peashooter/peashooter_idle_animation/peashooter_idle_8.xpm"

#include "../../assets/plants/peashooter/peashooter_shooting_animation/peashooter_shooting_1.xpm"
#include "../../assets/plants/peashooter/peashooter_shooting_animation/peashooter_shooting_2.xpm"
#include "../../assets/plants/peashooter/peashooter_shooting_animation/peashooter_shooting_3.xpm"

static xpm_map_t peashooter_idle_xpms[] = {
    (xpm_map_t) peashooter_idle_1,
    (xpm_map_t) peashooter_idle_2,
    (xpm_map_t) peashooter_idle_3,
    (xpm_map_t) peashooter_idle_4,
    (xpm_map_t) peashooter_idle_5,
    (xpm_map_t) peashooter_idle_6,
    (xpm_map_t) peashooter_idle_7,
    (xpm_map_t) peashooter_idle_8
};

static xpm_map_t peashooter_shooting_xpms[] = {
    (xpm_map_t) peashooter_shooting_1,
    (xpm_map_t) peashooter_shooting_2,
    (xpm_map_t) peashooter_shooting_3
};

static AnimSprite peashooter_idle_sprite;
static AnimSprite peashooter_shooting_sprite;
static uint16_t peashooter_shooting_frame_widths[PEASHOOTER_SHOOTING_FRAMES];
static uint16_t peashooter_shooting_frame_heights[PEASHOOTER_SHOOTING_FRAMES];

int load_peashooter_sprites() {
    if (sprite_load_frames(&peashooter_idle_sprite, peashooter_idle_xpms,
                           PEASHOOTER_IDLE_FRAMES) != 0) {
        return 1;
    }

    if (sprite_load_frames_with_dimensions(&peashooter_shooting_sprite,
                                           peashooter_shooting_xpms,
                                           peashooter_shooting_frame_widths,
                                           peashooter_shooting_frame_heights,
                                           PEASHOOTER_SHOOTING_FRAMES) != 0) {
        free_peashooter_sprites();
        return 1;
    }

    return 0;
}

void free_peashooter_sprites() {
    sprite_free_frames(&peashooter_idle_sprite);
    sprite_free_frames(&peashooter_shooting_sprite);
}

AnimSprite* get_peashooter_idle_sprite() {
    return &peashooter_idle_sprite;
}

AnimSprite* get_peashooter_shooting_sprite() {
    return &peashooter_shooting_sprite;
}

uint16_t get_peashooter_shooting_frame_width(int frame) {
    return sprite_frame_width(&peashooter_shooting_sprite,
                              peashooter_shooting_frame_widths,
                              frame);
}

uint16_t get_peashooter_shooting_frame_height(int frame) {
    return sprite_frame_height(&peashooter_shooting_sprite,
                               peashooter_shooting_frame_heights,
                               frame);
}
