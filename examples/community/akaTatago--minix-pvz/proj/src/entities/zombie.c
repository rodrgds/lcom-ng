#include "zombie.h"
#include "v_card.h"
#include "game_state.h"
#include "board.h"
#include <stdio.h>
#include "sprites/zombies/basic/zombie_basic_walking_1.xpm"
#include "sprites/zombies/basic/zombie_basic_walking_2.xpm"
#include "sprites/zombies/basic/zombie_basic_walking_3.xpm"
#include "sprites/zombies/basic/zombie_basic_walking_4.xpm"
#include "sprites/zombies/basic/zombie_basic_walking_5.xpm"
#include "sprites/zombies/basic/zombie_basic_walking_6.xpm"
#include "sprites/zombies/basic/zombie_basic_walking_7.xpm"
#include "animated_sprite.h"

zombie_t zombies[MAX_ZOMBIES];

static struct animated_sprite zombie_walking_sprite;

void zombies_init() {
    for (int i = 0; i < MAX_ZOMBIES; i++) zombies[i].active = false;

    char **zombie_walk_frames[] = {
        zombie_basic_walking_1_xpm,
        zombie_basic_walking_2_xpm,
        zombie_basic_walking_3_xpm,
        zombie_basic_walking_4_xpm,
        zombie_basic_walking_5_xpm,
        zombie_basic_walking_6_xpm,
        zombie_basic_walking_7_xpm,
    };

    init_animated_sprite(&zombie_walking_sprite, 7, 6, zombie_walk_frames, XPM_8_8_8_8);
}

void spawn_zombie(uint8_t lane, int level) {
    for (int i = 0; i < MAX_ZOMBIES; i++) {
        if (!zombies[i].active) {
            zombies[i].active = true;
            zombies[i].x = 780;
            zombies[i].lane = lane;
            zombies[i].hp = 200 + level * 20;
            zombies[i].speed = 2;
            zombies[i].eating = false;
            zombies[i].eat_timer = 0;
            return;
        }
    }
}

bool zombies_update() {
    for (int z = 0; z < MAX_ZOMBIES; z++) {
        if (!zombies[z].active || frozen) continue;

        int col = (zombies[z].x - GRID_START_X) / CELL_W;
        int row = zombies[z].lane;

        // eat plant
        if (col >= 0 && col < GRID_COLS && !board_is_empty(row, col) &&
            zombies[z].x <= GRID_START_X + (col + 1) * CELL_W + 55) {
            
            zombies[z].eating = true;
            zombies[z].eat_timer++;
            if (zombies[z].eat_timer >= 60) {
                zombies[z].eat_timer = 0;
                board_take_damage(row, col, 100);
            }
        } else {
            zombies[z].eating = false;
            zombies[z].eat_timer = 0;
            if (timer_counter % 7 == 0) zombies[z].x -= zombies[z].speed;
        }

        // game over
        if (zombies[z].x <= GRID_START_X - 30) return true;
    }
    return false;
}

void zombies_render() {
    for (int z = 0; z < MAX_ZOMBIES; z++) {
        if (!zombies[z].active || get_animated_sprite_pixmap(&zombie_walking_sprite) == NULL) continue;
        int draw_y = GRID_START_Y + zombies[z].lane * CELL_H + CELL_H - get_animated_sprite_image(&zombie_walking_sprite).height - 10;
        vg_draw_xpm(get_animated_sprite_image(&zombie_walking_sprite), get_animated_sprite_pixmap(&zombie_walking_sprite), zombies[z].x, draw_y);
    }
    tick_animated_sprite(&zombie_walking_sprite);
}

int zombies_alive_count() {
    int c = 0;
    for (int i = 0; i < MAX_ZOMBIES; i++) if (zombies[i].active) c++;
    return c;
}

bool zombie_in_lane_ahead(uint8_t lane, int16_t plant_x) {
    for (int i = 0; i < MAX_ZOMBIES; i++) {
        if (!zombies[i].active) continue;
        
        if (zombies[i].lane == lane && zombies[i].x > plant_x) {
            return true;
        }
    }
    return false;
}

bool zombies_check_hit(uint8_t lane, int16_t proj_x, int16_t damage) {
    for (int i = 0; i < MAX_ZOMBIES; i++) {
        if (!zombies[i].active) continue;

        if (zombies[i].lane == lane) {
            if (proj_x >= zombies[i].x && proj_x <= (zombies[i].x + 50)) {
                
                zombies[i].hp -= damage;
                
                if (zombies[i].hp <= 0) {
                    zombies[i].active = false;
                    score += 10;
                }
                
                return true;
            }
        }
    }
    return false;
}
