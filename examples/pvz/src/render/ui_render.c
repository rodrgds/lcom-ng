#include "pvz_platform.h"
#include <stdio.h>
#include <stdbool.h>
#include "board.h"
#include "render.h"
#include "state.h"
#include "scene.h"
#include "input.h"
#include "seed_bank.h"
#include "constants.h"

static AnimSprite *plant_sprite_for_type(PlantType type) {
    switch (type) {
        case SUNFLOWER: return get_sunflower_idle_sprite();
        case PEASHOOTER: return get_peashooter_idle_sprite();
        case WALL_NUT: return get_wall_nut_idle_sprite();
        case REPEATER: return get_repeater_idle_sprite();
        case CABBAGE: return get_cabbage_card_sprite();
        case POTATO_MINE: return get_potato_mine_idle_sprite();
        case SPIKEWEED: return get_spikeweed_card_sprite();
    }
    return NULL;
}

static AnimSprite *plant_preview_sprite_for_type(PlantType type) {
    switch (type) {
        case SUNFLOWER: return get_sunflower_idle_sprite();
        case PEASHOOTER: return get_peashooter_idle_sprite();
        case WALL_NUT: return get_wall_nut_idle_sprite();
        case REPEATER: return get_repeater_idle_sprite();
        case CABBAGE: return get_cabbage_idle_sprite();
        case POTATO_MINE: return get_potato_mine_idle_sprite();
        case SPIKEWEED: return get_spikeweed_idle_sprite();
    }
    return NULL;
}

static void draw_plant_target_cross(int row, int col) {
    int x;
    int y;
    int center_x;
    int center_y;
    int color = 0xFFFFFF;

    if (row < 0 || row >= BOARD_ROWS || col < 0 || col >= BOARD_COLS) return;

    x = COL_START_X[col];
    y = ROW_START_Y[row];
    center_x = x + 32;
    center_y = y + 40;

    vg_draw_rectangle(center_x - 15, center_y - 1, 31, 3, color);
    vg_draw_rectangle(center_x - 1, center_y - 15, 3, 31, color);
}

void draw_hud(void) {
    AnimSprite *hud = get_hud_sprite();
    if (hud != NULL && hud->pixmaps != NULL && hud->pixmaps[0] != NULL) {
        vg_draw_pixmap(hud->pixmaps[0], hud->width, hud->height, HUD_X, HUD_Y);
    }
}

void draw_seed_bank(PlantType selected_type) {
    int slot = get_seed_slot_for_type(selected_type);
    if (slot < 0) return;

    int x = get_seed_slot_x(slot) + 1;
    int y = SEED_BANK_Y + SEED_PACKET_Y_OFFSET + SEED_PACKET_H + 1;

    vg_draw_rectangle(x + 5, y, SEED_PACKET_W - 10, 3, 0x2B1A08);
}

static void draw_single_seed_card(int slot, PlantType type) {
    AnimSprite *packet = get_seed_packet_sprite();
    AnimSprite *plant_sprite = plant_sprite_for_type(type);
    const GameInfo *info = get_game_info();
    int cost = get_plant_cost(type);
    bool can_afford = info->sun_count >= cost;
    int x = get_seed_slot_x(slot) + (SEED_BANK_SLOT_W - SEED_PACKET_W) / 2;
    int y = SEED_BANK_Y + SEED_PACKET_Y_OFFSET;

    if (packet != NULL && packet->pixmaps != NULL && packet->pixmaps[0] != NULL) {
        vg_draw_pixmap(packet->pixmaps[0], packet->width, packet->height, x, y);
    }

    if (plant_sprite != NULL &&
        plant_sprite->pixmaps != NULL &&
        plant_sprite->pixmaps[0] != NULL) {
        int draw_x = x + SEED_PACKET_W / 2 - plant_sprite->width / 2;
        int draw_y = type == SPIKEWEED ? y + 18 : y + 7;
        if (type == CABBAGE) {
            draw_y -= 10;
        }
        vg_draw_pixmap(plant_sprite->pixmaps[0], plant_sprite->width, plant_sprite->height,
                       draw_x, draw_y);
    }

    char cost_str[8];
    sprintf(cost_str, "%d", cost);

    int len = 0;
    while (cost_str[len] != '\0') len++;

    int text_x = x + SEED_PACKET_W / 2 - (len * 6) / 2;
    int text_y = y + SEED_PACKET_H - 15;
    draw_string(text_x, text_y, cost_str, 1, can_afford ? 0x000000 : 0xC00000);
}

void draw_seed_cards(void) {
    for (int i = 0; i < SEED_BANK_SLOTS; i++) {
        draw_single_seed_card(i, get_seed_slot_type(i));
    }
}

void draw_shovel(int x, int y) {
    AnimSprite *shovel = get_shovel_sprite();
    if (shovel != NULL && shovel->pixmaps != NULL && shovel->pixmaps[0] != NULL) {
        vg_draw_pixmap(shovel->pixmaps[0], shovel->width, shovel->height, x, y);
    }
}

void draw_shovel_cursor(int mouse_x, int mouse_y) {
    AnimSprite *shovel = get_only_shovel_sprite();
    if (shovel != NULL && shovel->pixmaps != NULL && shovel->pixmaps[0] != NULL) {
        int x = mouse_x - shovel->width / 2;
        int y = mouse_y - shovel->height / 2;
        vg_draw_pixmap(shovel->pixmaps[0], shovel->width, shovel->height, x, y);
    }
}

void draw_shovel_highlight(int x, int y) {
    vg_draw_rectangle(x + 5, y + SHOVEL_H + 1, SHOVEL_W - 10, 3, 0x2B1A08);
}

void draw_holding_plant_preview(int mouse_x, int mouse_y) {
    PlantType type = get_selected_plant_type();
    AnimSprite *sprite = plant_preview_sprite_for_type(type);

    if (sprite != NULL && sprite->pixmaps != NULL && sprite->pixmaps[0] != NULL) {
        int draw_x = mouse_x - sprite->width / 2;
        int draw_y = mouse_y - sprite->height / 2;
        vg_draw_pixmap(sprite->pixmaps[0], sprite->width, sprite->height, draw_x, draw_y);
    }
}

void draw_keyboard_grid_selection(int row, int col) {
    int x;
    int y;
    int center_x;
    int center_y;
    int color = 0xFFFFFF;

    if (row < 0 || row >= BOARD_ROWS || col < 0 || col >= BOARD_COLS) return;

    x = COL_START_X[col];
    y = ROW_START_Y[row];
    center_x = x + 32;
    center_y = y + 40;

    vg_draw_rectangle(center_x - 1, center_y - 1, 3, 3, color);
    vg_draw_rectangle(center_x - 10, center_y - 1, 7, 3, color);
    vg_draw_rectangle(center_x + 4, center_y - 1, 7, 3, color);
    vg_draw_rectangle(center_x - 1, center_y - 10, 3, 7, color);
    vg_draw_rectangle(center_x - 1, center_y + 4, 3, 7, color);
}

void draw_keyboard_plant_preview(int row, int col, PlantType type) {
    (void) type;
    draw_plant_target_cross(row, col);
}
