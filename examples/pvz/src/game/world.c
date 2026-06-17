#include "world.h"

#include "board.h"
#include "combat.h"
#include "constants.h"
#include "input.h"
#include "lawnmower.h"
#include "plant_system.h"
#include "projectile_system.h"
#include "render.h"
#include "scene.h"
#include "state.h"
#include "wave.h"

static int zombie_screen_x(Zombie *zombie) {
    return COL_START_X[GRID_COLS - 1] + (int)zombie->curr_position;
}

static void handle_zombie_house_entry(Zombie *zombie) {
    if (zombie_screen_x(zombie) >= GRID_START_X) {
        return;
    }

    zombie->alive = false;

    if (!try_activate_lawnmower(zombie->row) && !is_lawnmower_rolling(zombie->row)) {
        set_game_state(STATE_GAME_OVER);
    }
}

static void update_zombies(Zombie *zombies, int max_zombies) {
    for (int i = 0; i < max_zombies; i++) {
        if (is_zombie_alive(&zombies[i])) {
            update_zombie_animation(&zombies[i]);
            move_zombie(&zombies[i]);
            handle_zombie_house_entry(&zombies[i]);
        }
    }
}

static void draw_zombies(Zombie *zombies, int max_zombies) {
    for (int i = 0; i < max_zombies; i++) {
        if (is_zombie_alive(&zombies[i])) {
            draw_zombie(&zombies[i]);
        }
    }
}

static void draw_shovel_card_ui(void) {
    if (is_shovel_mode_active()) {
        draw_shovel(SHOVEL_X, SHOVEL_Y);
        draw_shovel_highlight(SHOVEL_X, SHOVEL_Y);
    } else {
        draw_shovel(SHOVEL_X, SHOVEL_Y);
    }
}

static void draw_shovel_cursor_ui(void) {
    if (is_shovel_mode_active()) {
        if (!is_keyboard_grid_active()) {
            draw_shovel_cursor(get_mouse_x(), get_mouse_y());
        }
    }
}

static void draw_game_ui(void) {
    draw_hud();
    draw_sun_counter();
    draw_seed_cards();
    /* Only highlight the card while actively holding a plant. */
    if (get_mouse_state() == MOUSE_STATE_HOLDING_PLANT) {
        draw_seed_bank(get_selected_seed_type());
    }
}

static void draw_keyboard_grid_ui(void) {
    int row;
    int col;

    if (!is_keyboard_grid_active()) {
        return;
    }

    row = get_keyboard_selected_row();
    col = get_keyboard_selected_col();

    if (get_mouse_state() == MOUSE_STATE_HOLDING_PLANT) {
        draw_keyboard_plant_preview(row, col, get_selected_plant_type());
    } else if (is_shovel_mode_active()) {
        draw_shovel_cursor(COL_START_X[col] + 32, ROW_START_Y[row] + 40);
    } else {
        draw_keyboard_grid_selection(row, col);
    }
}

void update_world(Zombie *zombies, int max_zombies) {
    Board *board = get_board();

    update_plants(board, zombies, max_zombies);
    update_combat(board, zombies, max_zombies);
    update_projectiles(board, zombies, max_zombies);
    spawn_sun_if_needed();
    update_suns();

    update_wave_system(zombies, max_zombies);
    update_zombies(zombies, max_zombies);
    update_lawnmowers(zombies, max_zombies);
}

void draw_world(Zombie *zombies, int max_zombies) {
    Board *board = get_board();

    draw_map();
    draw_game_ui();
    draw_shovel_card_ui();
    draw_board();
    draw_lawnmowers();
    draw_zombies(zombies, max_zombies);
    draw_projectiles(board);
    draw_shovel_cursor_ui();
    draw_keyboard_grid_ui();

    if (get_mouse_state() == MOUSE_STATE_HOLDING_PLANT && !is_keyboard_grid_active()) {
        draw_holding_plant_preview(get_mouse_x(), get_mouse_y());
    }

    draw_wave_banner(get_wave_state()->banner_type, get_wave_state()->banner_timer);
}

void draw_paused_world(Zombie *zombies, int max_zombies) {
    draw_world(zombies, max_zombies);
    render_scene();
}
