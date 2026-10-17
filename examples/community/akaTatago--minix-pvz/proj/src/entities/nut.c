#include "nut.h"
#include "board.h"
#include "sprites/plants/nut/nut_idle_1.xpm"
#include "sprites/plants/nut/nut_idle_2.xpm"
#include "sprites/plants/nut/nut_idle_3.xpm"
#include "sprites/plants/nut/nut_idle_4.xpm"
#include "sprites/plants/nut/nut_idle_5.xpm"
#include "game_state.h"
#include "board.h"
#include "v_card.h"
#include "animated_sprite.h"


#define MAX_NUTS 45
static struct animated_sprite wallnut_idle_sprite;

typedef struct {
    bool active;
    int row;
    int col;
    int16_t hp;
} nut_t;

static nut_t nuts[MAX_NUTS];

void nuts_init(void) {
    for (int i = 0; i < MAX_NUTS; i++) {
        nuts[i].active = false;
        nuts[i].hp = 0;
    }
    
    char **wallnut_idle_frames[] = {
        wallnut_idle_1_xpm,
        wallnut_idle_2_xpm,
        wallnut_idle_3_xpm,
        wallnut_idle_4_xpm,
        wallnut_idle_5_xpm,
    };

    init_animated_sprite(&wallnut_idle_sprite, 5, 5, wallnut_idle_frames, XPM_8_8_8_8);
}

bool nuts_spawn(int row, int col) {
    for (int i = 0; i < MAX_NUTS; i++) {
        if (!nuts[i].active) {
            nuts[i].active = true;
            nuts[i].row = row;
            nuts[i].col = col;
            nuts[i].hp = NUT_MAX_HP;
            return true;
        }
    }
    return false;
}

void nuts_update(void) {
    for (int i = 0; i < MAX_NUTS; i++) {
        if (!nuts[i].active) continue;
    }
}

void nuts_render(void) {
    if (get_animated_sprite_pixmap(&wallnut_idle_sprite) == NULL) return;
    for (int i = 0; i < MAX_NUTS; i++) {
        if (!nuts[i].active) continue;
        int cell_x = GRID_START_X + nuts[i].col * CELL_W;
        int cell_y = GRID_START_Y + nuts[i].row * CELL_H;
        int draw_x = cell_x + (CELL_W - get_animated_sprite_image(&wallnut_idle_sprite).width) / 2;
        int draw_y = cell_y + (CELL_H - get_animated_sprite_image(&wallnut_idle_sprite).height) / 2;
        vg_draw_xpm(get_animated_sprite_image(&wallnut_idle_sprite), get_animated_sprite_pixmap(&wallnut_idle_sprite), draw_x, draw_y);
    }
    tick_animated_sprite(&wallnut_idle_sprite);
}

void nuts_take_damage(int row, int col, int damage) {
    for (int i = 0; i < MAX_NUTS; i++) {
        if (nuts[i].active && nuts[i].row == row && nuts[i].col == col) {
            nuts[i].hp -= damage;
            if (nuts[i].hp <= 0) {
                nuts[i].active = false;
                nuts[i].hp = 0;
                board_clear_cell(row, col); 
            }
            return;
        }
    }
}