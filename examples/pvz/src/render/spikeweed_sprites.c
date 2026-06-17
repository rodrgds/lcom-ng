#include "pvz_platform.h"
#include "sprite_loader.h"
#include "sprites.h"
#include "constants.h"

#include "../../assets/plants/spikeweed/weed1.xpm"
#include "../../assets/plants/spikeweed/weed2.xpm"
#include "../../assets/plants/spikeweed/weed3.xpm"
#include "../../assets/plants/spikeweed/weed4.xpm"
#include "../../assets/plants/spikeweed/weed5.xpm"
#include "../../assets/plants/spikeweed/weed6.xpm"
#include "../../assets/plants/spikeweed/weed7.xpm"
#include "../../assets/plants/spikeweed/weed_card.xpm"

static xpm_map_t spikeweed_idle_xpms[] = {
    (xpm_map_t) weed2_xpm,
    (xpm_map_t) weed3_xpm,
    (xpm_map_t) weed4_xpm
};

static xpm_map_t spikeweed_attack_xpms[] = {
    (xpm_map_t) weed1_xpm,
    (xpm_map_t) weed5_xpm,
    (xpm_map_t) weed6_xpm,
    (xpm_map_t) weed7_xpm
};

static xpm_map_t spikeweed_card_xpms[] = {
    (xpm_map_t) weed_card_xpm
};

static AnimSprite spikeweed_idle_sprite;
static AnimSprite spikeweed_attack_sprite;
static AnimSprite spikeweed_card_sprite;
static uint16_t spikeweed_idle_widths[SPIKEWEED_IDLE_FRAMES];
static uint16_t spikeweed_idle_heights[SPIKEWEED_IDLE_FRAMES];
static uint16_t spikeweed_attack_widths[SPIKEWEED_ATTACK_FRAMES];
static uint16_t spikeweed_attack_heights[SPIKEWEED_ATTACK_FRAMES];
static uint16_t spikeweed_card_widths[1];
static uint16_t spikeweed_card_heights[1];

int load_spikeweed_sprites() {
    if (sprite_load_frames_with_dimensions(&spikeweed_idle_sprite,
                                           spikeweed_idle_xpms,
                                           spikeweed_idle_widths,
                                           spikeweed_idle_heights,
                                           SPIKEWEED_IDLE_FRAMES) != 0) {
        free_spikeweed_sprites();
        return 1;
    }

    if (sprite_load_frames_with_dimensions(&spikeweed_attack_sprite,
                                           spikeweed_attack_xpms,
                                           spikeweed_attack_widths,
                                           spikeweed_attack_heights,
                                           SPIKEWEED_ATTACK_FRAMES) != 0) {
        free_spikeweed_sprites();
        return 1;
    }

    if (sprite_load_frames_with_dimensions(&spikeweed_card_sprite,
                                           spikeweed_card_xpms,
                                           spikeweed_card_widths,
                                           spikeweed_card_heights,
                                           1) != 0) {
        free_spikeweed_sprites();
        return 1;
    }

    return 0;
}

void free_spikeweed_sprites() {
    sprite_free_frames(&spikeweed_idle_sprite);
    sprite_free_frames(&spikeweed_attack_sprite);
    sprite_free_frames(&spikeweed_card_sprite);
}

AnimSprite* get_spikeweed_idle_sprite() {
    return &spikeweed_idle_sprite;
}

AnimSprite* get_spikeweed_attack_sprite() {
    return &spikeweed_attack_sprite;
}

AnimSprite* get_spikeweed_card_sprite() {
    return &spikeweed_card_sprite;
}

uint16_t get_spikeweed_idle_frame_width(int frame) {
    return sprite_frame_width(&spikeweed_idle_sprite, spikeweed_idle_widths, frame);
}

uint16_t get_spikeweed_idle_frame_height(int frame) {
    return sprite_frame_height(&spikeweed_idle_sprite, spikeweed_idle_heights, frame);
}

uint16_t get_spikeweed_attack_frame_width(int frame) {
    return sprite_frame_width(&spikeweed_attack_sprite, spikeweed_attack_widths, frame);
}

uint16_t get_spikeweed_attack_frame_height(int frame) {
    return sprite_frame_height(&spikeweed_attack_sprite, spikeweed_attack_heights, frame);
}
