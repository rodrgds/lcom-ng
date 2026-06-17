#include "board.h"

#include <stdlib.h>

#include "constants.h"
#include "render.h"
#include "state.h"

static Board board;

void init_board(void) {
    for (int r = 0; r < BOARD_ROWS; r++) {
        for (int c = 0; c < BOARD_COLS; c++) {
            board.cells[r][c].is_occupied = false;
            board.cells[r][c].x = COL_START_X[c];
            board.cells[r][c].y = ROW_START_Y[r];
        }
    }

    for (int i = 0; i < MAX_BULLETS; i++) {
        board.bullets[i].active = false;
    }

    for (int i = 0; i < MAX_SUNS; i++) {
        board.suns[i].is_active = false;
    }

    board.sun_spawn_timer = SUN_INITIAL_SPAWN_TIMER;
}

Board* get_board(void) {
    return &board;
}

void spawn_sun_at(int x, int y) {
    for (int i = 0; i < MAX_SUNS; i++) {
        if (!board.suns[i].is_active) {
            init_sun(&board.suns[i], x, y, y + 40);
            return;
        }
    }
}

void spawn_sun_if_needed(void) {
    if (board.sun_spawn_timer > 0) {
        board.sun_spawn_timer--;
        return;
    }

    for (int i = 0; i < MAX_SUNS; i++) {
        if (!board.suns[i].is_active) {
            int start_x = FALLING_SUN_START_X_MIN + rand() % FALLING_SUN_START_X_RANGE;
            int target_y = FALLING_SUN_TARGET_Y_MIN + rand() % FALLING_SUN_TARGET_Y_RANGE;
            init_sun(&board.suns[i], start_x, 0, target_y);
            break;
        }
    }

    board.sun_spawn_timer = SUN_SPAWN_TIMER_MIN + rand() % SUN_SPAWN_TIMER_RANGE;
}

void update_suns(void) {
    for (int i = 0; i < MAX_SUNS; i++) {
        if (!board.suns[i].is_active) {
            continue;
        }

        update_sun_animation(&board.suns[i]);

        if (board.suns[i].is_flying) {
            /* Move sun towards HUD counter */
            update_sun_fly(&board.suns[i]);

            /* Credit and deactivate once it reaches the target */
            if (board.suns[i].x == SUN_HUD_TARGET_X &&
                board.suns[i].y == SUN_HUD_TARGET_Y) {
                board.suns[i].is_active = false;
                add_sun(SUN_VALUE);
            }
            continue;
        }

        if (board.suns[i].y < board.suns[i].target_y) {
            board.suns[i].y += board.suns[i].speed;
            if (board.suns[i].y > board.suns[i].target_y) {
                board.suns[i].y = board.suns[i].target_y;
            }
        } else {
            board.suns[i].ground_ticks++;
            if (board.suns[i].ground_ticks >= SUN_GROUND_DURATION_TICKS) {
                board.suns[i].is_active = false;
            }
        }
    }
}

bool check_sun_click(int mouse_x, int mouse_y) {
    for (int i = 0; i < MAX_SUNS; i++) {
        if (board.suns[i].is_active && !board.suns[i].is_flying) {
            int center_x = board.suns[i].x + SUN_CLICK_CENTER_OFFSET;
            int center_y = board.suns[i].y + SUN_CLICK_CENTER_OFFSET;

            if (abs(mouse_x - center_x) <= SUN_CLICK_RADIUS &&
                abs(mouse_y - center_y) <= SUN_CLICK_RADIUS) {
                /* Start fly-to-HUD animation instead of immediate removal */
                board.suns[i].is_flying = true;
                board.suns[i].ground_ticks = 0; /* stop expiry timer */
                return true;
            }
        }
    }

    return false;
}

bool check_sun_collect_at_grid(int row, int col) {
    int cell_x;
    int cell_y;
    int cell_right;
    int cell_bottom;

    if (row < 0 || row >= BOARD_ROWS || col < 0 || col >= BOARD_COLS) {
        return false;
    }

    cell_x = COL_START_X[col];
    cell_y = ROW_START_Y[row];
    cell_right = cell_x + GRID_CELL_W;
    cell_bottom = cell_y + GRID_CELL_H;

    for (int i = 0; i < MAX_SUNS; i++) {
        int sun_left;
        int sun_top;
        int sun_right;
        int sun_bottom;

        if (!board.suns[i].is_active || board.suns[i].is_flying) {
            continue;
        }

        sun_left = board.suns[i].x;
        sun_top = board.suns[i].y;
        sun_right = sun_left + SUN_CLICK_CENTER_OFFSET * 2;
        sun_bottom = sun_top + SUN_CLICK_CENTER_OFFSET * 2;

        if (sun_right > cell_x && sun_left < cell_right &&
            sun_bottom > cell_y && sun_top < cell_bottom) {
            board.suns[i].is_flying = true;
            board.suns[i].ground_ticks = 0;
            return true;
        }
    }

    return false;
}

void draw_board(void) {
    for (int r = 0; r < BOARD_ROWS; r++) {
        for (int c = 0; c < BOARD_COLS; c++) {
            if (board.cells[r][c].is_occupied) {
                draw_plant(&board.cells[r][c].plant);
            }
        }
    }

    for (int i = 0; i < MAX_SUNS; i++) {
        if (board.suns[i].is_active) {
            draw_sun(board.suns[i].x, board.suns[i].y, board.suns[i].frame);
        }
    }
}

bool place_plant(int row, int col, PlantType type) {
    if (row < 0 || row >= BOARD_ROWS || col < 0 || col >= BOARD_COLS) return false;
    if (board.cells[row][col].is_occupied) return false;

    int cost = get_plant_cost(type);
    if (spend_sun(cost)) {
        init_plant(&board.cells[row][col].plant, row, col, type);
        board.cells[row][col].is_occupied = true;
        return true;
    }

    return false;
}

void remove_plant(int row, int col) {
    if (row < 0 || row >= BOARD_ROWS || col < 0 || col >= BOARD_COLS) {
        return;
    }

    if (!board.cells[row][col].is_occupied) {
        return;
    }

    board.cells[row][col].is_occupied = false;
}

bool get_grid_from_mouse(int mouse_x, int mouse_y, int *row, int *col) {
    *row = -1;
    *col = -1;

    for (int c = 0; c < BOARD_COLS; c++) {
        if (mouse_x >= COL_START_X[c] && mouse_x < COL_START_X[c] + GRID_CELL_W) {
            *col = c;
            break;
        }
    }

    for (int r = 0; r < BOARD_ROWS; r++) {
        if (mouse_y >= ROW_START_Y[r] && mouse_y < ROW_START_Y[r] + GRID_CELL_H) {
            *row = r;
            break;
        }
    }

    return *row != -1 && *col != -1;
}
