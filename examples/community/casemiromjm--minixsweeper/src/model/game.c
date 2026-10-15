#include "game.h"

int elapsed_seconds = 0;
int game_start_seconds = 0;
int game_end_seconds = 0;

void init_game(GameDifficulty difficulty) {
    unsigned int rows, cols, bombs;
    select_difficulty(difficulty, &rows, &cols, &bombs);
    int map_width = cols * CELL_SIZE;
    int map_height = rows * CELL_SIZE;
    unsigned int start_x = (SCREEN_WIDTH - map_width) / 2;
    unsigned int start_y = (SCREEN_HEIGHT - map_height) / 2 + BORDER_SPACING;
    map = create_map(rows, cols, bombs, start_x, start_y);
    rtc_update_time();
    game_start_seconds = time_info.hours * 3600 + time_info.minutes * 60 + time_info.seconds;
    elapsed_seconds = 0;
}

void select_difficulty(GameDifficulty difficulty, unsigned int *rows, unsigned int *cols, unsigned int *bombs) {
    switch (difficulty) {
        case EASY:
            *rows = 8; *cols = 8; *bombs = 10;
            break;
        case MEDIUM:
            *rows = 12; *cols = 12; *bombs = 25;
            break;
        case HARD:
            *rows = 16; *cols = 16; *bombs = 50;
            break;
    }
}

void on_game_end() {
    rtc_update_time();
    game_end_seconds = time_info.hours * 3600 + time_info.minutes * 60 + time_info.seconds;
    elapsed_seconds = game_end_seconds - game_start_seconds;
    if (elapsed_seconds < 0) elapsed_seconds += 24 * 3600;
}

int get_elapsed_seconds() {
    return elapsed_seconds;
}

void set_elapsed_seconds(int seconds) {
    elapsed_seconds = seconds;
}

