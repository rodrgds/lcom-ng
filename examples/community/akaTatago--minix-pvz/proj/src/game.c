#include "game.h"
#include "menu.h"
#include "v_card.h"
#include "i8042.h"
#include "proj.h"
#include <stdlib.h>
#include "font.h"
#include "rtc.h"
#include "levels.h"
#include "game_state.h"
#include "board.h"
#include "background.h"
#include "hud.h"
#include "sunflower.h"
#include "peashooter.h"
#include "sprites/misc/shovel.xpm"
#include "zombie.h"
#include "sun.h"
#include "projectile.h"
#include "wave_manager.h"
#include "name_input.h"
#include "scoreboard.h"
#include "plants_cooldown.h"
#include "nut.h"
 

int suns_count = 150;
int score = 0;
int current_level = 1;
bool frozen = false;

#define SCREEN_W 800
#define SCREEN_H 600

#define COLOR_BG      0x228B22
#define COLOR_CURSOR  0xFFFFFF
#define SQ_SIZE 60
#define SQ_GAP 20
#define SQ_START_X 210
#define SQ_START_Y 200

static GameState current_state = MAIN_MENU;

// mouse state
static int16_t mouse_x, mouse_y;
static bool prev_lb = false; 
#define CURSOR_SIZE 8

static bool quit = false;

// game local state
static int selected_plant = 1;
static uint32_t freeze_timer = 0;
static int suns_spent = 0;
static bool shovel_mode = false;
static xpm_image_t shovel_img;
static uint8_t *shovel_pixmap = NULL;

// RTC
static uint32_t rtc_timer = 0;
static rtc_date current_time;

void game_change_state(GameState new_state) {
    current_state = new_state;
}

void game_set_quit() {
    quit = true;
}

void game_init() {
    mouse_x = SCREEN_W / 2;
    mouse_y = SCREEN_H / 2;

    name_input_init(); // Kept from incoming branch
    scoreboard_init();

    suns_count = 150;
    score = 0;
    current_level = 1;
    selected_plant = 1;
    frozen = false;
    freeze_timer = 0;
    rtc_timer = 0;
    suns_spent = 0;
    shovel_mode = false;

    shovel_pixmap = xpm_load((xpm_map_t) shovel_xpm, XPM_8_8_8_8, &shovel_img);

    background_init();
    board_init();
    sunflowers_init();
    peashooters_init();
    nuts_init();
    zombies_init();
    projectiles_init();
    suns_init();
    cooldowns_init();

    hud_init();

    rtc_read_date(&current_time);
    srand(current_time.seconds + current_time.minutes * 60);
}

void game_init_level(int level_number) {
    board_init();
    sunflowers_init();
    peashooters_init();
    nuts_init();
    zombies_init();
    projectiles_init();
    suns_init();
    cooldowns_init();

    suns_count = 150; 
    score = 0;
    suns_spent = 0;
    selected_plant = 1;
    frozen = false;
    freeze_timer = 0;
    shovel_mode = false;

    current_level = level_number;
    
    // Using the modularized wave manager from HEAD
    wave_manager_init(level_number);
}

void game_update() {
    if (current_state == PLAYING) {
        wave_manager_update();

        if (wave_manager_is_complete()) {
            rtc_date now;
            rtc_read_date(&now);
            
            scoreboard_add(
                 name_input_get_name(),
                current_level,
                score,
                suns_spent,
                &now
    );
            game_change_state(LEVEL_COMPLETE);
        }

        if (frozen) {
            if (freeze_timer > 0) freeze_timer--;
            if (freeze_timer == 0) frozen = false;
        }

        sunflowers_update();
        peashooters_update();
        nuts_update();
        projectiles_update();
        suns_update();
        cooldowns_update();

        bool game_over = zombies_update();

        if (game_over) {
            rtc_read_date(&current_time);
            current_state = GAME_OVER;
        }    


        rtc_timer++;
        if (rtc_timer >= 60) {
            rtc_read_date(&current_time);
            rtc_timer = 0;
        }
    }
}

void game_render() {
    if (current_state == MAIN_MENU) {
        menu_render(mouse_x, mouse_y);
    }
    else if (current_state == NAME_INPUT) {
        name_input_render();
    }
    else if (current_state == LEVEL_SELECT) {
        level_select_render(mouse_x, mouse_y);
    }
    else if (current_state == PLAYING) {
        vg_clear(COLOR_BG);

        background_render();

        sunflowers_render();
        peashooters_render();
        nuts_render();
        zombies_render();
        projectiles_render();
        suns_render();

        // Render modular UI from HEAD
        ui_render_hud(suns_count, score, current_level, selected_plant, frozen, shovel_mode, &current_time);

        // Render player name from incoming branch
        const char *player_name = name_input_get_name();
        if (player_name[0] != '\0') {
            font_draw_string(player_name, 500, 40, 0xCCCCCC);
        }
    }
    else if (current_state == LEVEL_COMPLETE) {
        vg_clear(COLOR_BG);
        background_render();
        ui_render_hud(suns_count, score, current_level, selected_plant, frozen, shovel_mode, &current_time);
        ui_render_level_complete(suns_spent);
    }
    else if (current_state == GAME_OVER) {
        vg_clear(COLOR_BG);
        background_render();
        ui_render_hud(suns_count, score, current_level, selected_plant, frozen, shovel_mode, &current_time);
        ui_render_game_over(suns_spent);
    }

    else if (current_state == SCOREBOARD) {
        scoreboard_render();
    }

    if (current_state == PLAYING && shovel_mode && shovel_pixmap != NULL) {
        int draw_x = mouse_x - shovel_img.width / 2;
        int draw_y = mouse_y - shovel_img.height / 2;
        vg_draw_xpm(shovel_img, shovel_pixmap, draw_x, draw_y);
    } else {
        vg_draw_rectangle(mouse_x - CURSOR_SIZE / 2, mouse_y - 1, CURSOR_SIZE, 2, COLOR_CURSOR);
        vg_draw_rectangle(mouse_x - 1, mouse_y - CURSOR_SIZE / 2, 2, CURSOR_SIZE, COLOR_CURSOR);
    }

    vg_flip();
}

bool game_should_quit() {
    return quit;
}

void game_on_key(uint8_t scancode) {
    if (current_state == MAIN_MENU) {
        menu_on_key(scancode);
    } 

    else if (current_state == NAME_INPUT) {
        if (name_input_on_key(scancode)) {
            game_change_state(PLAYING);
        }

        if (scancode == ESC_BREAKCODE) {
            game_change_state(MAIN_MENU);
        }
    }

    else if (current_state == LEVEL_SELECT) {
        if (scancode == ESC_BREAKCODE) { 
            current_state = MAIN_MENU;
        }
    }
    else if (current_state == PLAYING) {
        if (scancode == ESC_BREAKCODE) {
            current_state = MAIN_MENU;
            shovel_mode = false;
        }

        if (scancode == 0x02) selected_plant = 1;
        if (scancode == 0x03) selected_plant = 2;
        if (scancode == 0x04) selected_plant = 3;
        if (scancode == 0x13) {
            shovel_mode = !shovel_mode;
        }

        if (scancode == 0x39 && !frozen && suns_count >= 10) {
            suns_count -= 10;
            suns_spent += 10;
            frozen = true;
            freeze_timer = 180;
        }

    } else if (current_state == LEVEL_COMPLETE) {
        if (scancode == ESC_BREAKCODE) {
            current_state = MAIN_MENU;
        }

        if (scancode == 0x1C && current_level < 10) { 
            game_init_level(current_level + 1);
            game_change_state(PLAYING);
        }
    } else if (current_state == GAME_OVER) {
        if (scancode == ESC_BREAKCODE) {
            current_state = MAIN_MENU;
        }
    }
    else if (current_state == SCOREBOARD) {
        if (scancode == ESC_BREAKCODE) {
            current_state = MAIN_MENU;
        }
    }
    
}

void game_on_mouse(struct packet *pp) {
    mouse_x += pp->delta_x;
    mouse_y -= pp->delta_y;

    if (mouse_x < 0) mouse_x = 0;
    if (mouse_x >= SCREEN_W) mouse_x = SCREEN_W - 1;
    if (mouse_y < 0) mouse_y = 0;
    if (mouse_y >= SCREEN_H) mouse_y = SCREEN_H - 1;

    if (current_state == MAIN_MENU) {
        menu_on_mouse(pp, mouse_x, mouse_y);
    } 
    else if (current_state == LEVEL_SELECT) {
        static bool lvl_prev_lb = false;
        if (pp->lb && !lvl_prev_lb) {
            for (int i = 0; i < 10; i++) {
                int row = i / 5;
                int col = i % 5;
                int bx = SQ_START_X + col * (SQ_SIZE + SQ_GAP);
                int by = SQ_START_Y + row * (SQ_SIZE + SQ_GAP);

                if (mouse_x >= bx && mouse_x <= bx + SQ_SIZE &&
                    mouse_y >= by && mouse_y <= by + SQ_SIZE) {
                    
                    game_init_level(i + 1); 
                    game_change_state(NAME_INPUT);
                    break;
                }
            }

            if (mouse_x >= 300 && mouse_x <= 500 && mouse_y >= 410 && mouse_y <= 460) {
                game_change_state(MAIN_MENU);
            }
        }
        lvl_prev_lb = pp->lb;
    }
    else if (current_state == PLAYING) {
        if (pp->lb && !prev_lb) {
            
            bool sun_collected = suns_check_click(mouse_x, mouse_y);

            if (!sun_collected) {
                if (mouse_x >= GRID_START_X && mouse_x < GRID_START_X + GRID_COLS * CELL_W &&
                    mouse_y >= GRID_START_Y && mouse_y < GRID_START_Y + GRID_ROWS * CELL_H) {

                    int col = (mouse_x - GRID_START_X) / CELL_W;
                    int row = (mouse_y - GRID_START_Y) / CELL_H;

                    if (shovel_mode) {
                        if (board_get_plant_type(row, col) != PLANT_NONE) {
                            board_take_damage(row, col, 10000);
                        }
                    } else {
                        int cost = 0;
                        plant_type_t p_type = PLANT_NONE;

                        if (selected_plant == 1) {
                            cost = 50;
                            p_type = PLANT_SUNFLOWER;
                        } else if (selected_plant == 2) {
                            cost = 100;
                            p_type = PLANT_PEASHOOTER;
                        } else if (selected_plant == 3) { // Add this block for the Nut
                            cost = 50; // Standard cost for a Wall-nut
                            p_type = PLANT_NUT;
                        }

                        if (p_type != PLANT_NONE && suns_count >= cost && cooldown_is_ready(p_type)) {
                            if (board_place_plant(row, col, p_type)) {
                                suns_count -= cost;
                                suns_spent += cost;
                                cooldown_start(p_type);
                            }
                        }
                    }
                }
            }
        }
    }

    prev_lb = pp->lb;
}
