#include "pvz_platform.h"
#include "sprite_loader.h"
#include "sprites.h"
#include "constants.h"

#include "../../assets/plants/potatoes/idle/idle1.xpm"
#include "../../assets/plants/potatoes/idle/idle2.xpm"
#include "../../assets/plants/potatoes/idle/idle3.xpm"
#include "../../assets/plants/potatoes/idle/idle4.xpm"
#include "../../assets/plants/potatoes/idle/idle5.xpm"
#include "../../assets/plants/potatoes/pop/pop1_potatoe.xpm"
#include "../../assets/plants/potatoes/pop/pop2.xpm"
#include "../../assets/plants/potatoes/pop/pop3.xpm"
#include "../../assets/plants/potatoes/explosion/exp1.xpm"
#include "../../assets/plants/potatoes/explosion/exp2.xpm"
#include "../../assets/plants/potatoes/explosion/exp3.xpm"
#include "../../assets/plants/potatoes/explosion/exp4.xpm"
#include "../../assets/plants/potatoes/explosion/exp5.xpm"
#include "../../assets/plants/potatoes/explosion/exp6.xpm"
#include "../../assets/plants/potatoes/explosion/exp7.xpm"
#include "../../assets/plants/potatoes/explosion/exp8.xpm"

static xpm_map_t potato_mine_idle_xpms[] = {
    (xpm_map_t) idle1_xpm,
    (xpm_map_t) idle2_xpm,
    (xpm_map_t) idle3_xpm,
    (xpm_map_t) idle4_xpm,
    (xpm_map_t) idle5_xpm
};

static xpm_map_t potato_mine_pop_xpms[] = {
    (xpm_map_t) pop1_potatoe_xpm,
    (xpm_map_t) pop2_xpm,
    (xpm_map_t) pop3_xpm
};

static xpm_map_t potato_mine_explosion_xpms[] = {
    (xpm_map_t) exp1_xpm,
    (xpm_map_t) exp2_xpm,
    (xpm_map_t) exp3_xpm,
    (xpm_map_t) exp4_xpm,
    (xpm_map_t) exp5_xpm,
    (xpm_map_t) exp6_xpm,
    (xpm_map_t) exp7_xpm,
    (xpm_map_t) exp8_xpm
};

static AnimSprite potato_mine_idle_sprite;
static AnimSprite potato_mine_pop_sprite;
static AnimSprite potato_mine_explosion_sprite;

static uint16_t potato_mine_idle_widths[POTATO_MINE_IDLE_FRAMES];
static uint16_t potato_mine_idle_heights[POTATO_MINE_IDLE_FRAMES];
static uint16_t potato_mine_pop_widths[POTATO_MINE_POP_FRAMES];
static uint16_t potato_mine_pop_heights[POTATO_MINE_POP_FRAMES];
static uint16_t potato_mine_explosion_widths[POTATO_MINE_EXPLOSION_FRAMES];
static uint16_t potato_mine_explosion_heights[POTATO_MINE_EXPLOSION_FRAMES];

int load_potato_mine_sprites() {
    if (sprite_load_frames_with_dimensions(&potato_mine_idle_sprite,
                                           potato_mine_idle_xpms,
                                           potato_mine_idle_widths,
                                           potato_mine_idle_heights,
                                           POTATO_MINE_IDLE_FRAMES) != 0) {
        free_potato_mine_sprites();
        return 1;
    }

    if (sprite_load_frames_with_dimensions(&potato_mine_pop_sprite,
                                           potato_mine_pop_xpms,
                                           potato_mine_pop_widths,
                                           potato_mine_pop_heights,
                                           POTATO_MINE_POP_FRAMES) != 0) {
        free_potato_mine_sprites();
        return 1;
    }

    if (sprite_load_frames_with_dimensions(&potato_mine_explosion_sprite,
                                           potato_mine_explosion_xpms,
                                           potato_mine_explosion_widths,
                                           potato_mine_explosion_heights,
                                           POTATO_MINE_EXPLOSION_FRAMES) != 0) {
        free_potato_mine_sprites();
        return 1;
    }

    return 0;
}

void free_potato_mine_sprites() {
    sprite_free_frames(&potato_mine_idle_sprite);
    sprite_free_frames(&potato_mine_pop_sprite);
    sprite_free_frames(&potato_mine_explosion_sprite);
}

AnimSprite* get_potato_mine_idle_sprite() {
    return &potato_mine_idle_sprite;
}

AnimSprite* get_potato_mine_pop_sprite() {
    return &potato_mine_pop_sprite;
}

AnimSprite* get_potato_mine_explosion_sprite() {
    return &potato_mine_explosion_sprite;
}

uint16_t get_potato_mine_idle_frame_width(int frame) {
    return sprite_frame_width(&potato_mine_idle_sprite, potato_mine_idle_widths, frame);
}

uint16_t get_potato_mine_idle_frame_height(int frame) {
    return sprite_frame_height(&potato_mine_idle_sprite, potato_mine_idle_heights, frame);
}

uint16_t get_potato_mine_pop_frame_width(int frame) {
    return sprite_frame_width(&potato_mine_pop_sprite, potato_mine_pop_widths, frame);
}

uint16_t get_potato_mine_pop_frame_height(int frame) {
    return sprite_frame_height(&potato_mine_pop_sprite, potato_mine_pop_heights, frame);
}

uint16_t get_potato_mine_explosion_frame_width(int frame) {
    return sprite_frame_width(&potato_mine_explosion_sprite,
                              potato_mine_explosion_widths,
                              frame);
}

uint16_t get_potato_mine_explosion_frame_height(int frame) {
    return sprite_frame_height(&potato_mine_explosion_sprite,
                               potato_mine_explosion_heights,
                               frame);
}
