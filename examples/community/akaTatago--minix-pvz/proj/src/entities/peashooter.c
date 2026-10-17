#include "peashooter.h"
#include "game_state.h"
#include "board.h"
#include "projectile.h"
#include "zombie.h"
#include "v_card.h"
#include <stdio.h>
#include "animated_sprite.h"
#include "sprites/plants/peashooter/peashooter_idle_1.xpm"
#include "sprites/plants/peashooter/peashooter_idle_2.xpm"
#include "sprites/plants/peashooter/peashooter_idle_3.xpm"
#include "sprites/plants/peashooter/peashooter_idle_4.xpm"
#include "sprites/plants/peashooter/peashooter_idle_5.xpm"
#include "sprites/plants/peashooter/peashooter_idle_6.xpm"
#include "sprites/plants/peashooter/peashooter_idle_7.xpm"
#include "sprites/plants/peashooter/peashooter_idle_8.xpm"

#define MAX_PEASHOOTERS 45

typedef struct {
    bool active;
    int row, col;
    int16_t hp;
    uint16_t shoot_timer;
} peashooter_t;

static peashooter_t peashooters[MAX_PEASHOOTERS];
static struct animated_sprite peashooter_idle_sprite;

void peashooters_init() {
    for (int i = 0; i < MAX_PEASHOOTERS; i++) peashooters[i].active = false;

    char **peashooter_idle_frames[] = {
        peashooter_idle_1_xpm,
        peashooter_idle_2_xpm,
        peashooter_idle_3_xpm,
        peashooter_idle_4_xpm,
        peashooter_idle_5_xpm,
        peashooter_idle_6_xpm,
        peashooter_idle_7_xpm,
        peashooter_idle_8_xpm,
    };

    init_animated_sprite(&peashooter_idle_sprite, 8, 5, peashooter_idle_frames, XPM_8_8_8_8);
}

bool peashooters_spawn(int row, int col) {
    for (int i = 0; i < MAX_PEASHOOTERS; i++) {
        if (!peashooters[i].active) {
            peashooters[i].active = true;
            peashooters[i].row = row;
            peashooters[i].col = col;
            peashooters[i].hp = 300;
            peashooters[i].shoot_timer = 0;
            return true;
        }
    }
    return false;
}

void peashooters_update() {
    for (int i = 0; i < MAX_PEASHOOTERS; i++) {
        if (!peashooters[i].active) continue;

        peashooters[i].shoot_timer++;

        // shoot if zombie in front
        if (peashooters[i].shoot_timer >= 84) {
            peashooters[i].shoot_timer = 0;

            int plant_x = GRID_START_X + peashooters[i].col * CELL_W;
            if (zombie_in_lane_ahead(peashooters[i].row, plant_x)) {
                int px = plant_x + CELL_W;
                int py = GRID_START_Y + peashooters[i].row * CELL_H + CELL_H / 2 - 15;
                spawn_projectile(peashooters[i].row, px, py);
            }
        }
    }
}

void peashooters_render() {
    if (get_animated_sprite_pixmap(&peashooter_idle_sprite) == NULL) return;
    for (int i = 0; i < MAX_PEASHOOTERS; i++) {
        if (!peashooters[i].active) continue;
        int cell_x = GRID_START_X + peashooters[i].col * CELL_W;
        int cell_y = GRID_START_Y + peashooters[i].row * CELL_H;
        int draw_x = cell_x + (CELL_W - get_animated_sprite_image(&peashooter_idle_sprite).width) / 2;
        int draw_y = cell_y + (CELL_H - get_animated_sprite_image(&peashooter_idle_sprite).height) / 2;
        vg_draw_xpm(get_animated_sprite_image(&peashooter_idle_sprite), get_animated_sprite_pixmap(&peashooter_idle_sprite), draw_x, draw_y);
    }
    tick_animated_sprite(&peashooter_idle_sprite);
}

void peashooters_take_damage(int row, int col, int damage) {
    for (int i = 0; i < MAX_PEASHOOTERS; i++) {
        if (peashooters[i].active && peashooters[i].row == row && peashooters[i].col == col) {
            peashooters[i].hp -= damage;
            if (peashooters[i].hp <= 0) {
                peashooters[i].active = false;
                board_clear_cell(row, col);
            }
            return;
        }
    }
}
