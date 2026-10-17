#include "sunflower.h"
#include "game_state.h"
#include "board.h"
#include "sun.h"
#include "v_card.h"
#include <stdio.h>
#include <stdlib.h>
#include "sprites/plants/sunflower/sunflower_idle_1.xpm"
#include "sprites/plants/sunflower/sunflower_idle_2.xpm"
#include "sprites/plants/sunflower/sunflower_idle_3.xpm"
#include "sprites/plants/sunflower/sunflower_idle_4.xpm"
#include "sprites/plants/sunflower/sunflower_idle_5.xpm"
#include "sprites/plants/sunflower/sunflower_idle_6.xpm"
#include "animated_sprite.h"

#define MAX_SUNFLOWERS 45

typedef struct {
    bool active;
    int row, col;
    int16_t hp;
    uint16_t sun_timer;
} sunflower_t;

static sunflower_t sunflowers[MAX_SUNFLOWERS];
static struct animated_sprite sunflower_idle_sprite;

void sunflowers_init() {
    for (int i = 0; i < MAX_SUNFLOWERS; i++) sunflowers[i].active = false;
    
    char **sunflower_idle_frames[] = {
        sunflower_idle_1_xpm,
        sunflower_idle_2_xpm,
        sunflower_idle_3_xpm,
        sunflower_idle_4_xpm,
        sunflower_idle_5_xpm,
        sunflower_idle_6_xpm,
        sunflower_idle_6_xpm,
        sunflower_idle_5_xpm,
        sunflower_idle_4_xpm,
        sunflower_idle_3_xpm,
        sunflower_idle_2_xpm,
        sunflower_idle_1_xpm,
    };

    init_animated_sprite(&sunflower_idle_sprite, 12, 5, sunflower_idle_frames, XPM_8_8_8_8);
}

bool sunflowers_spawn(int row, int col) {
    for (int i = 0; i < MAX_SUNFLOWERS; i++) {
        if (!sunflowers[i].active) {
            sunflowers[i].active = true;
            sunflowers[i].row = row;
            sunflowers[i].col = col;
            sunflowers[i].hp = 300;
            sunflowers[i].sun_timer = 0;
            return true;
        }
    }
    return false;
}

void sunflowers_update() {
    for (int i = 0; i < MAX_SUNFLOWERS; i++) {
        if (!sunflowers[i].active) continue;

        sunflowers[i].sun_timer++;
        if (sunflowers[i].sun_timer >= 840) {
            sunflowers[i].sun_timer = 0;
            int sx = GRID_START_X + sunflowers[i].col * CELL_W + 20 + (rand() % 40);
            int sy = GRID_START_Y + sunflowers[i].row * CELL_H + 20 + (rand() % 40);
            spawn_sun(sx, sy, sy);
        }
    }
}

void sunflowers_render() {
    if (get_animated_sprite_pixmap(&sunflower_idle_sprite) == NULL) return;
    for (int i = 0; i < MAX_SUNFLOWERS; i++) {
        if (!sunflowers[i].active) continue;
        int cell_x = GRID_START_X + sunflowers[i].col * CELL_W;
        int cell_y = GRID_START_Y + sunflowers[i].row * CELL_H;
        int draw_x = cell_x + (CELL_W - get_animated_sprite_image(&sunflower_idle_sprite).width) / 2;
        int draw_y = cell_y + (CELL_H - get_animated_sprite_image(&sunflower_idle_sprite).height) / 2;
        vg_draw_xpm(get_animated_sprite_image(&sunflower_idle_sprite), get_animated_sprite_pixmap(&sunflower_idle_sprite), draw_x, draw_y);
    }
    tick_animated_sprite(&sunflower_idle_sprite);
}

void sunflowers_take_damage(int row, int col, int damage) {
    for (int i = 0; i < MAX_SUNFLOWERS; i++) {
        if (sunflowers[i].active && sunflowers[i].row == row && sunflowers[i].col == col) {
            sunflowers[i].hp -= damage;
            if (sunflowers[i].hp <= 0) {
                sunflowers[i].active = false;
                board_clear_cell(row, col);
            }
            return;
        }
    }
}
