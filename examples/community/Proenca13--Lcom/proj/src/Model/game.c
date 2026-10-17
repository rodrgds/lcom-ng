#include "game.h"

extern Sprite *grass_sprite;
extern Sprite *tnt_sprite;
extern Sprite *selected_grass_sprite;
extern Sprite *cobblestone_sprite;
extern Sprite *selected_cobblestone_sprite;
extern Sprite *tile1_sprite;
extern Sprite *selected_tile1_sprite;
extern Sprite *tile2_sprite;
extern Sprite *selected_tile2_sprite;
extern Sprite *tile3_sprite;
extern Sprite *selected_tile3_sprite;
extern Sprite *tile4_sprite;
extern Sprite *selected_tile4_sprite;
extern Sprite *tile5_sprite;
extern Sprite *selected_tile5_sprite;
extern Sprite *tile6_sprite;
extern Sprite *selected_tile6_sprite;
extern Sprite *tile7_sprite;
extern Sprite *selected_tile7_sprite;
extern Sprite *tile8_sprite;
extern Sprite *selected_tile8_sprite;
extern Sprite *flag_sprite;
extern Sprite *selected_flag_sprite;
uint8_t rows = 8;
uint8_t cols = 8;
Grid_entry grid_entry = {0, 0};
Block* **grid;

Block *create_block(Sprite *block_sprite,Sprite *selected_block_sprite,BlockType type, int16_t row, int16_t col,bool is_selected,BlockState state){
    Block *block = (Block *) malloc (sizeof(Block));
    block->block_sprite = block_sprite;
    block->selected_block_sprite = selected_block_sprite;
    block->type= type;
    block->row = row;
    block->col = col;
    block->is_selected = is_selected;
    block->state = state;
    return block;
}

void placeBombs(){
    int bombs_placed = 0;
    while (bombs_placed < 10) {
        int random_row = rand() % rows;
        int random_col = rand() % cols;

        if (grid[random_row][random_col]->type != BOMB && (random_row!= grid_entry.x && random_col!=grid_entry.y)) {
            grid[random_row][random_col]->type = BOMB;
            grid[random_row][random_col]->block_sprite = tnt_sprite;
            grid[random_row][random_col]->selected_block_sprite = tnt_sprite;
            bombs_placed++;
        }
    }
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if(grid[i][j]->type != BOMB){
                count_bombs_around( i , j) ;
            }
        }
    }
}

void count_bombs_around(int16_t x , int16_t y){
    uint8_t count = 0;
    int dr[] = {-1, -1, -1, 0, 0, 1, 1, 1};
    int dc[] = {-1, 0, 1, -1, 1, -1, 0, 1};
    for (int k = 0; k < 8; k++) {
        int n_row = x + dr[k];
        int n_col = y + dc[k];
        if (n_row >= 0 && n_row < rows && n_col >= 0 && n_col < cols) {
            if (grid[n_row][n_col]->type == BOMB) {
                count++;
            }
        }
    }
    switch (count) {
        case 0:
            grid[x][y]->type = EMPTY;
            grid[x][y]->block_sprite = cobblestone_sprite;
            grid[x][y]->selected_block_sprite = selected_cobblestone_sprite;
            break;
        case 1:
            grid[x][y]->type = ONE;
            grid[x][y]->block_sprite = tile1_sprite;
            grid[x][y]->selected_block_sprite = selected_tile1_sprite;
            break;
        case 2:
            grid[x][y]->type = TWO;
            grid[x][y]->block_sprite = tile2_sprite;
            grid[x][y]->selected_block_sprite = selected_tile2_sprite;
            break;
        case 3:
            grid[x][y]->type = THREE;
            grid[x][y]->block_sprite = tile3_sprite;
            grid[x][y]->selected_block_sprite = selected_tile3_sprite;
            break;
        case 4:
            grid[x][y]->type = FOUR;
            grid[x][y]->block_sprite = tile4_sprite;
            grid[x][y]->selected_block_sprite = selected_tile4_sprite;
            break;
        case 5:
            grid[x][y]->type = FIVE;
            grid[x][y]->block_sprite = tile5_sprite;
            grid[x][y]->selected_block_sprite = selected_tile5_sprite;
            break;
        case 6:
            grid[x][y]->type = SIX;
            grid[x][y]->block_sprite = tile6_sprite;
            grid[x][y]->selected_block_sprite = selected_tile6_sprite;
            break;
        case 7:
            grid[x][y]->type = SEVEN;
            grid[x][y]->block_sprite = tile7_sprite;
            grid[x][y]->selected_block_sprite = selected_tile7_sprite;
            break;
        case 8:
            grid[x][y]->type = EIGHT;
            grid[x][y]->block_sprite = tile8_sprite;
            grid[x][y]->selected_block_sprite = selected_tile8_sprite;
            break;
    }
}

void create_game(){
    grid = malloc(rows * sizeof(Block *));
    for (int i = 0; i < rows; i++) {
        grid[i] = malloc(cols * sizeof(Block));
    }
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            Sprite *block_sprite = grass_sprite;
            Sprite *selected_block_sprite = selected_grass_sprite;
            BlockType type = EMPTY;
            int16_t row = i;
            int16_t col = j;
            bool is_selected = false;
            BlockState state = Not_Revealed;
            grid[i][j] = create_block(block_sprite,selected_block_sprite ,type, row, col, is_selected, state);
        }
    }
    grid[grid_entry.x][grid_entry.y]->is_selected = true;
}

void destroy_game(){
    for (int i = 0; i < rows; i++) {
        free(grid[i]);
    }
    free(grid);
}

int check_win(){
    uint8_t revealed_count = 0;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if(grid[i][j]->type != BOMB && grid[i][j]->state == Revealed){
                revealed_count++;
            }
        }
    }
    if(revealed_count == ((8*8)-10)) return 1;
    return 0;
}

void flag_counter(uint8_t *flag_count){
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if(grid[i][j]->state == Flagged){
                (*flag_count)++;
            }
        }
    }

}

int check_first_touch(){
    uint8_t revealed_count = 0;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if(grid[i][j]->state == Revealed){
                revealed_count++;
            }
        }
    }
    return revealed_count == 0;
}

int check_can_flag(){
    uint8_t flag_count = 0;
    flag_counter(&flag_count);
    if(flag_count >= 10)return 0;
    return 1;
}

void reveal_near_zeros(int16_t x , int16_t y){
            int dr[] = {-1, -1, -1, 0, 0, 1, 1, 1};
            int dc[] = {-1, 0, 1, -1, 1, -1, 0, 1};
            for (int k = 0; k < 8; k++) {
                int n_row = x + dr[k];
                int n_col = y + dc[k];
                if (n_row >= 0 && n_row < rows && n_col >= 0 && n_col < cols) {
                    if (grid[n_row][n_col]->state != Revealed) {
                        grid[n_row][n_col]->state = Revealed;
                        if(grid[n_row][n_col]->type == EMPTY){
                            reveal_near_zeros(n_row,n_col);
                        }
                    }
                }
    }
}
