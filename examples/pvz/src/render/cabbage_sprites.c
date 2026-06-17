#include "pvz_platform.h"
#include "sprite_loader.h"
#include "sprites.h"
#include "constants.h"

#include "../../assets/plants/cabbage/idle/cabbage_idle_1.xpm"
#include "../../assets/plants/cabbage/idle/cabbage_idle_2.xpm"
#include "../../assets/plants/cabbage/idle/cabbage_idle_3.xpm"
#include "../../assets/plants/cabbage/idle/cabbage_idle_4.xpm"
#include "../../assets/plants/cabbage/idle/cabbage_idle_5.xpm"
#include "../../assets/plants/cabbage/idle/cabbage_idle_6.xpm"

#include "../../assets/plants/cabbage/shooting/cabbage_shooting_1.xpm"
#include "../../assets/plants/cabbage/shooting/cabbage_shooting_2.xpm"
#include "../../assets/plants/cabbage/shooting/cabbage_shooting_3.xpm"
#include "../../assets/plants/cabbage/shooting/cabbage_shooting_4.xpm"
#include "../../assets/plants/cabbage/shooting/cabbage_shooting_5.xpm"
#include "../../assets/plants/cabbage/shooting/cabbage_shooting_6.xpm"
#include "../../assets/plants/cabbage/shooting/cabbage_shooting_7.xpm"
#include "../../assets/plants/cabbage/shooting/cabbage_shooting_8.xpm"
#include "../../assets/plants/cabbage/cabbage_card.xpm"

static xpm_map_t cabbage_idle_xpms[] = {
    (xpm_map_t) cabbage_idle_1_xpm,
    (xpm_map_t) cabbage_idle_2_xpm,
    (xpm_map_t) cabbage_idle_3_xpm,
    (xpm_map_t) cabbage_idle_4_xpm,
    (xpm_map_t) cabbage_idle_5_xpm,
    (xpm_map_t) cabbage_idle_6_xpm
};

static xpm_map_t cabbage_shooting_xpms[] = {
    (xpm_map_t) cabbage_shooting_1_xpm,
    (xpm_map_t) cabbage_shooting_2_xpm,
    (xpm_map_t) cabbage_shooting_3_xpm,
    (xpm_map_t) cabbage_shooting_4_xpm,
    (xpm_map_t) cabbage_shooting_5_xpm,
    (xpm_map_t) cabbage_shooting_6_xpm,
    (xpm_map_t) cabbage_shooting_7_xpm,
    (xpm_map_t) cabbage_shooting_8_xpm
};

static xpm_map_t cabbage_card_xpms[] = {
    (xpm_map_t) cabbage_card_xpm
};

static AnimSprite cabbage_idle_sprite;
static AnimSprite cabbage_shooting_sprite;
static AnimSprite cabbage_card_sprite;
static uint16_t cabbage_idle_frame_widths[CABBAGE_IDLE_FRAMES];
static uint16_t cabbage_idle_frame_heights[CABBAGE_IDLE_FRAMES];
static uint16_t cabbage_shooting_frame_widths[CABBAGE_SHOOTING_FRAMES];
static uint16_t cabbage_shooting_frame_heights[CABBAGE_SHOOTING_FRAMES];

int load_cabbage_sprites(void) {
    if (sprite_load_frames_with_dimensions(&cabbage_idle_sprite, cabbage_idle_xpms,
                                           cabbage_idle_frame_widths,
                                           cabbage_idle_frame_heights,
                                           CABBAGE_IDLE_FRAMES) != 0) {
        return 1;
    }

    if (sprite_load_frames_with_dimensions(&cabbage_shooting_sprite,
                                           cabbage_shooting_xpms,
                                           cabbage_shooting_frame_widths,
                                           cabbage_shooting_frame_heights,
                                           CABBAGE_SHOOTING_FRAMES) != 0) {
        free_cabbage_sprites();
        return 1;
    }

    if (sprite_load_frames(&cabbage_card_sprite, cabbage_card_xpms, 1) != 0) {
        free_cabbage_sprites();
        return 1;
    }

    return 0;
}

void free_cabbage_sprites(void) {
    sprite_free_frames(&cabbage_idle_sprite);
    sprite_free_frames(&cabbage_shooting_sprite);
    sprite_free_frames(&cabbage_card_sprite);
}

AnimSprite* get_cabbage_idle_sprite(void) {
    return &cabbage_idle_sprite;
}

AnimSprite* get_cabbage_shooting_sprite(void) {
    return &cabbage_shooting_sprite;
}

AnimSprite* get_cabbage_card_sprite(void) {
    return &cabbage_card_sprite;
}

uint16_t get_cabbage_idle_frame_width(int frame) {
    return sprite_frame_width(&cabbage_idle_sprite, cabbage_idle_frame_widths, frame);
}

uint16_t get_cabbage_idle_frame_height(int frame) {
    return sprite_frame_height(&cabbage_idle_sprite, cabbage_idle_frame_heights, frame);
}

uint16_t get_cabbage_shooting_frame_width(int frame) {
    return sprite_frame_width(&cabbage_shooting_sprite,
                              cabbage_shooting_frame_widths,
                              frame);
}

uint16_t get_cabbage_shooting_frame_height(int frame) {
    return sprite_frame_height(&cabbage_shooting_sprite,
                               cabbage_shooting_frame_heights,
                               frame);
}
