#include "map.h"

extern uint32_t timer_ticks;

Map* map = NULL;

/**
 * @brief Calculates the number of adjacent bombs for each cell in the map.
 * 
 * For each cell, counts bombs in the 8 surrounding cells (if any).
 * Skips cells that are bombs themselves.
 * 
 * @param map Pointer to the Map structure.
 */
static void map_adjacent_bombs(Map *map) {
    for (unsigned int row = 0; row < map->rows; row++) {
        for (unsigned int col = 0; col < map->cols; col++) {
            if (map->cells[row][col].is_bomb) continue;
            unsigned int count = 0;
            for (int dr = -1; dr <= 1; dr++) {
                for (int dc = -1; dc <= 1; dc++) {
                    if (dr == 0 && dc == 0) continue;
                    unsigned int nr = row + dr;
                    unsigned int nc = col + dc;
                    if (nr < map->rows && nc < map->cols) {
                        if (map->cells[nr][nc].is_bomb) count++;
                    }
                }
            }
            map->cells[row][col].adjacent_bombs = count;
        }
    }
}

Map* create_map(unsigned int rows, unsigned int cols, unsigned int bombs, unsigned int start_x, unsigned int start_y) {
    if (rows == 0 || cols == 0 || bombs > rows * cols) return NULL;

    Map* map = (Map*) malloc(sizeof(Map));
    if (!map) return NULL;

    map->rows = rows;
    map->cols = cols;
    map->total_bombs = bombs;
    map->start_x = start_x;
    map->start_y = start_y;
    map->flags_used = 0;
    map->revealed_cells = 0;
    map->lamp_used = false;

    map->cells = (Cell**) malloc(rows * sizeof(Cell*));

    for (unsigned int i = 0; i < rows; i++) {
        map->cells[i] = (Cell*) calloc(cols, sizeof(Cell)); // Initialize all cells to zero
        if (!map->cells[i]) { // On allocation failure, free previously allocated memory to avoid leaks
            for (unsigned int j = 0; j < i; j++) free(map->cells[j]);
            free(map->cells);
            free(map);
            return NULL;
        }
    }

    srand(timer_ticks); // Initialize random seed with timer_ticks

    unsigned int placed = 0;
    while (placed < bombs) {
        unsigned int r = rand() % rows;
        unsigned int c = rand() % cols;
        if (!map->cells[r][c].is_bomb) {
            map->cells[r][c].is_bomb = true;
            placed++;
        }
    }

    map_adjacent_bombs(map); // Calculate adjacent bombs count for all cells

    return map;
}

void destroy_map(Map *map) {
    if (!map) return;
    for (unsigned int i = 0; i < map->rows; i++) {
        free(map->cells[i]);
    }
    free(map->cells);
    free(map);
}

void map_reveal_cell(Map *map, unsigned int row, unsigned int col) {
    if (row >= map->rows || col >= map->cols) return;

    Cell *cell = &map->cells[row][col];
    if (cell->is_revealed || cell->is_flagged) return;

    cell->is_revealed = true;

    if (!cell->is_bomb) {
        map->revealed_cells++;

        // Check if player won
        if (map->revealed_cells == map->rows * map->cols - map->total_bombs) {
            // Reveal all cells
            for (unsigned int i = 0; i < map->rows; i++) {
                for (unsigned int j = 0; j < map->cols; j++) {
                    map->cells[i][j].is_revealed = true;
                }
            }
            return;
        }
    }

    // Recursively reveal adjacent cells if no bombs nearby
    if (!cell->is_bomb && cell->adjacent_bombs == 0) {
        for (int i = -1; i <= 1; i++) {
            for (int j = -1; j <= 1; j++) {
                if (i == 0 && j == 0) continue;
                map_reveal_cell(map, row + i, col + j);
            }
        }
    }
}

void map_reveal_all_bombs(Map *map) {
    for (unsigned int i = 0; i < map->rows; i++) {
        for (unsigned int j = 0; j < map->cols; j++) {
            Cell *cell = &map->cells[i][j];
            if (cell->is_bomb) {
                cell->is_revealed = true;
            }
        }
    }
}

void map_flag_cell(Map *map, unsigned int row, unsigned int col) {
    if (row >= map->rows || col >= map->cols) return;

    Cell *cell = &map->cells[row][col];
    if (cell->is_revealed) return;

    cell->is_flagged = !cell->is_flagged;

    if (cell->is_flagged) {
        map->flags_used++;
    } else {
        map->flags_used--;
    }
}

void reveal_random_cell(Map *map) {
    unsigned int valid_cells_count = 0;
    unsigned int rows = map->rows;
    unsigned int cols = map->cols;

    // Count valid cells
    for (unsigned int row = 0; row < rows; row++) {
        for (unsigned int col = 0; col < cols; col++) {
            Cell *cell = &map->cells[row][col];
            if (!cell->is_revealed && !cell->is_bomb && cell->adjacent_bombs > 0) {
                valid_cells_count++;
            }
        }
    }

    if (valid_cells_count == 0) {
        return;
    }

    unsigned int target_index = rand() % valid_cells_count;
    unsigned int current_index = 0;

    // Reveal the chosen random cell
    for (unsigned int row = 0; row < rows; row++) {
        for (unsigned int col = 0; col < cols; col++) {
            Cell *cell = &map->cells[row][col];
            if (!cell->is_revealed && !cell->is_bomb && cell->adjacent_bombs > 0) {
                if (current_index == target_index) {
                    map_reveal_cell(map, row, col);
                    return;
                }
                current_index++;
            }
        }
    }
}

void deceiver_change_cell(Map *map) {
    srand(timer_ticks); // Reinitialize seed

    unsigned int row = rand() % map->rows;
    unsigned int col = rand() % map->cols;

    while (map->cells[row][col].adjacent_bombs == 0 || map->cells[row][col].is_bomb) {
        row = rand() % map->rows;
        col = rand() % map->cols;
    }

    if (map->cells[row][col].adjacent_bombs == 1) {
        map->cells[row][col].adjacent_bombs++;
    }
    else {
        unsigned int number = rand() % 2;
        if (number == 0) {
            map->cells[row][col].adjacent_bombs--;
        }
        else {
            map->cells[row][col].adjacent_bombs++;
        }
    }
}

void map_first_reveal(Map *map, unsigned int row, unsigned int col) {
    // Ensure row and col are not on the map edges to avoid out-of-bounds when clearing neighbors
    if (row == 0) {
        row = 1;
    }
    else if (row == map->rows - 1) {
        row = map->rows - 2;
    }

    if (col == 0) {
        col = 1;
    }
    else if (col == map->cols - 1) {
        col = map->cols - 2;
    }

    srand(timer_ticks); // Reinitialize seed

    // Guarantee a 3x3 bomb-free area around the first clicked cell
    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            unsigned int new_row = row + i;
            unsigned int new_col = col + j;
            
            if (map->cells[new_row][new_col].is_bomb) {
                map->cells[new_row][new_col].is_bomb = false;
                unsigned int r = rand() % map->rows;
                unsigned int c = rand() % map->cols;
                // Find a new position for the bomb away from the 3x3 area
                while (map->cells[r][c].is_bomb || (abs((int)r - (int)row) <= 1 && abs((int)c - (int)col) <= 1)) {
                    r = rand() % map->rows;
                    c = rand() % map->cols;
                }
                
                map->cells[r][c].is_bomb = true;
            }
        }
    }

    map_adjacent_bombs(map); // Recalculate adjacent bomb counts after moves

    map_reveal_cell(map, row, col); // Reveal the first clicked cell
}

