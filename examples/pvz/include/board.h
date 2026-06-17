#ifndef _BOARD_H_
#define _BOARD_H_

#include <stdbool.h>

#include "bullet.h"
#include "constants.h"
#include "plant.h"
#include "sun.h"

typedef struct {
    bool is_occupied;
    Plant plant;
    int x;
    int y;
} GridCell;

typedef struct {
    GridCell cells[BOARD_ROWS][BOARD_COLS];
    Bullet bullets[MAX_BULLETS];
    Sun suns[MAX_SUNS];
    int sun_spawn_timer;
} Board;

void init_board(void);
Board* get_board(void);
void draw_board(void);

void spawn_sun_if_needed(void);
void update_suns(void);
bool check_sun_click(int mouse_x, int mouse_y);
bool check_sun_collect_at_grid(int row, int col);

bool place_plant(int row, int col, PlantType type);
void remove_plant(int row, int col);
void spawn_sun_at(int x, int y);

bool get_grid_from_mouse(int mouse_x, int mouse_y, int *row, int *col);

#endif
