#include "board.h"
#include "game_state.h"
#include "sunflower.h"
#include "peashooter.h"
#include <stddef.h>
#include "nut.h"

static plant_type_t grid[GRID_ROWS][GRID_COLS];

void board_init() {
    for (int r = 0; r < GRID_ROWS; r++) {
        for (int c = 0; c < GRID_COLS; c++) {
            grid[r][c] = PLANT_NONE;
        }
    }
}

bool board_is_empty(int row, int col) {
    if (row < 0 || row >= GRID_ROWS || col < 0 || col >= GRID_COLS) return false;
    return grid[row][col] == PLANT_NONE;
}

plant_type_t board_get_plant_type(int row, int col) {
    if (row < 0 || row >= GRID_ROWS || col < 0 || col >= GRID_COLS) return PLANT_NONE;
    return grid[row][col];
}

bool board_place_plant(int row, int col, plant_type_t type) {
    if (!board_is_empty(row, col)) return false;

    if (type == PLANT_SUNFLOWER) {
        if (sunflowers_spawn(row, col)) {
            grid[row][col] = PLANT_SUNFLOWER;
            return true;
        }
    } else if (type == PLANT_PEASHOOTER) {
        if (peashooters_spawn(row, col)) {
            grid[row][col] = PLANT_PEASHOOTER;
            return true;
        }
    } else if (type == PLANT_NUT) {
        if(nuts_spawn(row, col)) {
            grid[row][col] = PLANT_NUT;
            return true;
        }
    }
    return false;
}

void board_take_damage(int row, int col, int damage) {
    if (row < 0 || row >= GRID_ROWS || col < 0 || col >= GRID_COLS) return;

    if (grid[row][col] == PLANT_SUNFLOWER) {
        sunflowers_take_damage(row, col, damage);
    } else if (grid[row][col] == PLANT_PEASHOOTER) {
        peashooters_take_damage(row, col, damage);
    } else if (grid[row][col] == PLANT_NUT) { // Add this block
        nuts_take_damage(row, col, damage);
    }
}

void board_clear_cell(int row, int col) {
    if (row >= 0 && row < GRID_ROWS && col >= 0 && col < GRID_COLS) {
        grid[row][col] = PLANT_NONE;
    }
}
