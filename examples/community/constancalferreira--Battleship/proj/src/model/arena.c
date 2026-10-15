#include "arena.h"

extern vbe_mode_info_t mode_info;
extern struct packet mouse_packet; 
extern int cursor_x, cursor_y;
extern uint8_t *current_buffer;
uint8_t *arena_buffer;
extern unsigned int frame_size;
extern uint8_t scancode;

#define MAX_CELLS (GRID_ROWS * GRID_COLS)
#define BOMB_DELAY_FRAMES 20
static int pc_available_rows[MAX_CELLS];
static int pc_available_cols[MAX_CELLS];
static int pc_available_count = 0;
static int pc_bombs_to_play = 0;
static int pc_bomb_timer = 0;

Arena arena;
ArenaPhase arena_phase = SETUP_PLAYER1;
PlayerTurn current_player = PLAYER_1;
DragState drag_state = {0, -1, 0, 0, 0, 0};
extern GameMode game_mode;


#define MAX_BOMBS_PER_TURN 5
int bombs_remaining = MAX_BOMBS_PER_TURN;



void arena_main_loop() { 
  draw_arena();
  draw_cursor(current_buffer);
  swap_buffers();

  if (arena_phase == BATTLE_PHASE && game_mode == SINGLE_PLAYER && current_player == PLAYER_2) {
    battle_phase_pc(); 
  } 
}



void set_arena_buffer() {
  if (arena_buffer == NULL) {
    arena_buffer = (uint8_t *) malloc(frame_size);
  }
}



void init_arena() {

  arena_phase = SETUP_PLAYER1;
  current_player = PLAYER_1;

  //initialize grids
  init_grid(&arena.player1_grid, 10, 40);
  init_grid(&arena.player2_grid, 410, 40);

  //draw background
  set_arena_buffer();
  draw_arena_background(arena_buffer);

  //setup ships
  setup_ships(&arena.player1_grid);
  if (game_mode == MULTI_PLAYER) setup_ships(&arena.player2_grid);
  else if (game_mode == SINGLE_PLAYER) {
    setup_pc_ships(&arena.player2_grid);
    pc_init_available_cells(&arena.player1_grid);
  }

}


void init_grid(Grid *grid, int sprite_x, int sprite_y) {

  grid->sprite_x = sprite_x;
  grid->sprite_y = sprite_y;
  grid->x = sprite_x + 31; 
  grid->y = sprite_y + 31; 

  grid->ships_remaining = NUM_SHIPS;


  for (int i = 0; i < GRID_ROWS; i++) {
    for (int j = 0; j < GRID_COLS; j++) {
      grid->cells[i][j].state = EMPTY;
      grid->cells[i][j].ship_id = -1;
    }
  }

  for (int i = 0; i < NUM_SHIPS; i++) {
    grid->ships[i].type = NO_SHIP;
    grid->ships[i].size = 0;
    grid->ships[i].orientation = 0;
    grid->ships[i].start_row = -1;
    grid->ships[i].start_col = -1;
    grid->ships[i].status = ALIVE;
  }
}





void arena_keyboard_handler() {
  switch (scancode) {
    case R_KEY:
      if (arena_phase == SETUP_PLAYER1 && current_player == PLAYER_1) {
        if (game_mode == SINGLE_PLAYER) {
          arena_phase = BATTLE_PHASE;
        } else {
          current_player = PLAYER_2;
          arena_phase = SETUP_PLAYER2;
        }  
      } else if (arena_phase == SETUP_PLAYER2 && current_player == PLAYER_2) {
        current_player = PLAYER_1;
        arena_phase = BATTLE_PHASE;
      }
      break;
    default:
      break;
  }
}



void arena_mouse_handler() {
  static bool prev_lb = false;
  bool curr_lb = mouse_packet.lb;
  static bool prev_rb = false;
  bool curr_rb = mouse_packet.rb;

  if (arena_phase == SETUP_PLAYER1 && current_player == PLAYER_1) {
    setup_phase(curr_lb, prev_lb, curr_rb, prev_rb, &arena.player1_grid);
  } else if (arena_phase == SETUP_PLAYER2 && current_player == PLAYER_2) {
    setup_phase(curr_lb, prev_lb, curr_rb, prev_rb, &arena.player2_grid);
  } else if (arena_phase == BATTLE_PHASE) {
    battle_phase_mouse(curr_lb, prev_lb);
  }

  prev_lb = curr_lb;
}




//used in battle phase
bool handle_mouse_click(Grid *grid, int mouse_x, int mouse_y) {
  for (int i = 0; i < GRID_ROWS; i++) {
    for (int j = 0; j < GRID_COLS; j++) {
      int x, y;
      cell_to_pixel(grid, i, j, &x, &y);
      if (mouse_x >= x && mouse_x < x + CELL_WIDTH &&
          mouse_y >= y && mouse_y < y + CELL_HEIGHT) {
        
        Cell *cell = &grid->cells[i][j];
        
        if (cell->state == SHIP) {

          cell->state = HIT;
          if (game_mode == MULTI_PLAYER || current_player == PLAYER_1) {
            bombs_remaining--;
          }

          if (is_ship_sunk(grid, cell->ship_id)) {
            grid->ships[cell->ship_id].status = SUNK;
            grid->ships_remaining--;

            //check if any player has won
            if (grid->ships_remaining == 0) {
              set_state(GAME_OVER);
            }
          }

          return true;

        } else if (cell->state == EMPTY) {

          cell->state = MISS;
          if (game_mode == MULTI_PLAYER || current_player == PLAYER_1) {
            bombs_remaining--;
          }
          
          return true;
        }
        
        
      }
    }
  }
  return false;
}



/* BATTLE PHASE: Bombing */

//if it is a human playing, he will use the mouse to click on the grid
void battle_phase_mouse(bool curr_lb, bool prev_lb) {
  if (bombs_remaining > 0) { //check player still has bombsg
    if (curr_lb && !prev_lb) {
      if (current_player == PLAYER_1) {
        handle_mouse_click(&arena.player2_grid, cursor_x, cursor_y);
      } else if (current_player == PLAYER_2 && game_mode == MULTI_PLAYER) {
        handle_mouse_click(&arena.player1_grid, cursor_x, cursor_y);
      }
    }
  } else {
    if (current_player == PLAYER_1) {
      current_player = PLAYER_2;
    } else if (current_player == PLAYER_2) {
      current_player = PLAYER_1;
    }
    bombs_remaining = MAX_BOMBS_PER_TURN; //reset bombs for the new turn
  }
}


//if it is the pc playing, it will bomb randomly
void battle_phase_pc() {
  if (game_mode == SINGLE_PLAYER && current_player == PLAYER_2) {
    if (pc_bombs_to_play == 0 && pc_available_count > 0) {
      pc_bombs_to_play = MAX_BOMBS_PER_TURN;
      pc_bomb_timer = 0;
    }

    if (pc_bombs_to_play > 0 && pc_available_count > 0) {
      pc_bomb_timer++;
      if (pc_bomb_timer >= BOMB_DELAY_FRAMES) {
        pc_bomb_timer = 0;

        int idx = rand() % pc_available_count;
        int row = pc_available_rows[idx];
        int col = pc_available_cols[idx];

        //we don't have the position of the cursor here, so we have to simulate it
        int x, y;
        cell_to_pixel(&arena.player1_grid, row, col, &x, &y);

        if (handle_mouse_click(&arena.player1_grid, x, y)) {
          pc_available_rows[idx] = pc_available_rows[pc_available_count - 1];
          pc_available_cols[idx] = pc_available_cols[pc_available_count - 1];
          pc_available_count--;
          pc_bombs_to_play--;
        }
      }
    }

    if (pc_bombs_to_play == 0 || pc_available_count == 0) {
      current_player = PLAYER_1;
      bombs_remaining = MAX_BOMBS_PER_TURN;
    }
  }
}




/* SETUP PHASE: Drag and drop ships */
void setup_phase(bool curr_lb, bool prev_lb, bool curr_rb, bool prev_rb, Grid *grid) {


  //RIGHT BUTTON: Rotate ships

  if (curr_rb && !prev_rb && !drag_state.dragging) {
    int row, col, ship_id;
    if (mouse_over_ship(grid, cursor_x, cursor_y, &row, &col, &ship_id)) {
      Ship *ship = &grid->ships[ship_id];
      int size = ship->size;
      int old_ori = ship->orientation;
      int new_ori = 1 - old_ori;

      //we calculate the offset of the ship from its starting position
      int offset = (old_ori == 0) ? (col - ship->start_col) : (row - ship->start_row);

      //we compute the new starting position based on the offset
      int new_start_row = (new_ori == 0) ? row : row - offset;
      int new_start_col = (new_ori == 0) ? col - offset : col;


      //we check if the ship can be placed with the new orientation
      if (can_place_ship(grid, new_start_row, new_start_col, size, new_ori, ship_id)) {

        //we remove ship from old orientation
        for (int i = 0; i < size; i++) {
          int r = (ship->orientation == 0) ? ship->start_row : ship->start_row + i;
          int c = (ship->orientation == 0) ? ship->start_col + i : ship->start_col;
          grid->cells[r][c].state = EMPTY;
          grid->cells[r][c].ship_id = -1;
        }
        //and then we place ship in new orientation
        ship->orientation = new_ori;
        ship->start_row = new_start_row;
        ship->start_col = new_start_col;
        for (int i = 0; i < size; i++) {
          int r = (new_ori == 0) ? new_start_row : new_start_row + i;
          int c = (new_ori == 0) ? new_start_col + i : new_start_col;
          grid->cells[r][c].state = SHIP;
          grid->cells[r][c].ship_id = ship_id;
        }
      }
    }
  }
  prev_rb = curr_rb;





  //LEFT BUTTON: Drag and drop ships

  if (curr_lb && !prev_lb) {
    int row, col, ship_id;
    if (mouse_over_ship(grid, cursor_x, cursor_y, &row, &col, &ship_id)) {
      Ship *selected_ship = &grid->ships[ship_id];
      drag_state.dragging = true;
      drag_state.origin_row = row;
      drag_state.origin_col = col;
      drag_state.ship_id = ship_id;
      drag_state.offset = (selected_ship-> orientation== 0) ? (col - selected_ship->start_col) : (row - selected_ship->start_row);
      drag_state.active_grid = grid;
    }
  }

  //on mouse release, drop ship if dragging
  if (!curr_lb && prev_lb && drag_state.dragging) {
    int drop_row, drop_col;
    //we check if the mouse is over a cell
    if (mouse_over_cell(grid, cursor_x, cursor_y, &drop_row, &drop_col)) {
      
      Ship *dragged_ship = &grid->ships[drag_state.ship_id];
      int new_start_row = drop_row;
      int new_start_col = drop_col;

      if (dragged_ship->orientation == 0) {
        new_start_col -= drag_state.offset; 
      } else {
        new_start_row -= drag_state.offset; 
      }

      //if the ship can be placed, move it, else do nothing
      if (can_place_ship(grid, new_start_row, new_start_col, dragged_ship->size, dragged_ship->orientation, drag_state.ship_id)) {
        move_ship(grid, drag_state.ship_id, new_start_row, new_start_col);
      }
    }  
    
    drag_state.dragging = false;
    drag_state.active_grid = NULL;
  }
}




//we check if the mouse is over a ship
bool mouse_over_ship(Grid *grid, int mouse_x, int mouse_y, int *row, int *col, int *ship_id) {
  for (int i = 0; i < GRID_ROWS; i++) {
    for (int j = 0; j < GRID_COLS; j++) {
      int x, y;
      cell_to_pixel(grid, i, j, &x, &y);
      if (mouse_x >= x && mouse_x < x + CELL_WIDTH &&
        mouse_y >= y && mouse_y < y + CELL_HEIGHT &&
        grid->cells[i][j].state == SHIP) {
        *row = i;
        *col = j;
        *ship_id = grid->cells[i][j].ship_id;
        return true;
      }
    }
  }
  return false;
}

//we check if the mouse is over a grid cell
bool mouse_over_cell(Grid *grid, int mouse_x, int mouse_y, int *row, int *col) {
  for (int i = 0; i < GRID_ROWS; i++) {
    for (int j = 0; j < GRID_COLS; j++) {
      int x, y;
      cell_to_pixel(grid, i, j, &x, &y);
      if (mouse_x >= x && mouse_x < x + CELL_WIDTH &&
        mouse_y >= y && mouse_y < y + CELL_HEIGHT) {
        *row = i;
        *col = j;
        return true;
      }
    }
  }
  return false;
}





void move_ship(Grid *grid, int ship_id, int new_row, int new_col) {
    //get ship info
    Ship *ship = &grid->ships[ship_id];
    int size = ship->size;
    int old_row = ship->start_row;
    int old_col = ship->start_col;
    int orientation = ship->orientation;

    //we remove ship from old position
    for (int i = 0; i < size; i++) {
        int row = (orientation == 0) ? old_row : old_row + i;
        int col = (orientation == 0) ? old_col + i : old_col;
        grid->cells[row][col].state = EMPTY;
        grid->cells[row][col].ship_id = -1;
    }

    //we try to place at new position
    if (can_place_ship(grid, new_row, new_col, size, orientation, ship_id)) {
        //if we can, we place it at new position
        ship->start_row = new_row;
        ship->start_col = new_col;
        for (int i = 0; i < size; i++) {
            int row = (orientation == 0) ? new_row : new_row + i;
            int col = (orientation == 0) ? new_col + i : new_col;
            grid->cells[row][col].state = SHIP;
            grid->cells[row][col].ship_id = ship_id;
        }
    } else {
        //if invalid, restore to old position
        for (int i = 0; i < size; i++) {
            int row = (orientation == 0) ? old_row : old_row + i;
            int col = (orientation == 0) ? old_col + i : old_col;
            grid->cells[row][col].state = SHIP;
            grid->cells[row][col].ship_id = ship_id;
        }
        ship->start_row = old_row;
        ship->start_col = old_col;
    }
}



bool is_ship_sunk(Grid *grid, int ship_id) {
  Ship *ship = &grid->ships[ship_id];
  if (ship->type == NO_SHIP) return false;

  for (int i = 0; i < ship->size; i++) {
    int row = (ship->orientation == 0) ? ship->start_row : ship->start_row + i;
    int col = (ship->orientation == 0) ? ship->start_col + i : ship->start_col;
    if (grid->cells[row][col].state != HIT) {
      return false; //at least one part of the ship is not hit
    }
  }
  return true; //all parts of the ship are hit
}






void cell_to_pixel(Grid *grid, int row, int col, int* x, int* y) {
  *x = grid->x + (col * CELL_WIDTH) + col;
  *y = grid->y + (row * CELL_HEIGHT) + row;
}



int coord_to_cell(const char* coord, int* row, int* col) {

  if (coord == NULL || strlen(coord) < 2) {
    return 1; //error: invalid coordinate
  }

  char col_letter = coord[0];
  if (col_letter < 'A' || col_letter > 'J') {
    return 1; //error: invalid column letter
  }
  *col = col_letter - 'A';

  int row_num;
  if (coord[2] == '\0') {
    row_num = coord[1] - '0';
  } else if (coord[1] == '1' && coord[2] == '0') {
    row_num = 10;
  } else {
    return 1; //error: invalid row number
  } 

  *row = row_num -1; //converting to index (0-9)

  if (*row < 0 || *row >= GRID_ROWS) return 1;
  return 0; 
}




bool can_place_ship(Grid *grid, int start_row, int start_col, int size, int orientation, int ship_id) {
  if (start_row < 0 || start_col < 0) return false;

  if (orientation == 0) { //horizontal
    if (start_col + size > GRID_COLS || start_row >= GRID_ROWS) return false;
  } else { //vertical
    if (start_row + size > GRID_ROWS || start_col >= GRID_COLS) return false;
  }

  for (int i = 0; i < size; i++) {
    int row = (orientation == 0) ? start_row : start_row + i;
    int col = (orientation == 0) ? start_col + i : start_col;
    for (int dx = -1; dx <= 1; dx++) {
      for (int dy = -1; dy <= 1; dy++) {
        int check_row = row + dy;
        int check_col = col + dx;
        if (check_row < 0 || check_row >= GRID_ROWS ||
            check_col < 0 || check_col >= GRID_COLS)
          continue;
        if (grid->cells[check_row][check_col].state == SHIP && grid->cells[check_row][check_col].ship_id != ship_id)
          return false;
      }
    }
  }

  return true;
}



bool add_ship(Grid *grid, int ship_id, ShipType type, int orientation, const char* coord) {
  int row, col;
  int size = SHIP_TYPE_TO_SIZE(type);
  if (size == 0) return false;
  if (coord_to_cell(coord, &row, &col) != 0) return false;
  if (!can_place_ship(grid, row, col, size, orientation, ship_id)) return false;

  grid->ships[ship_id].type = type;
  grid->ships[ship_id].size = size;
  grid->ships[ship_id].orientation = orientation;
  grid->ships[ship_id].start_row = row;
  grid->ships[ship_id].start_col = col;

  for (int i = 0; i < size; i++) {
    int ship_row = (orientation == 0) ? row : row + i;
    int ship_col = (orientation == 0) ? col + i : col;
    grid->cells[ship_row][ship_col].state = SHIP;
    grid->cells[ship_row][ship_col].ship_id = ship_id;
  }
  return true;
}



void setup_ships(Grid *grid) {
  add_ship(grid, 0, SHIP_1, 0, "A1"); 
  add_ship(grid, 1, SHIP_1, 0, "E2");
  add_ship(grid, 2, SHIP_3, 0, "G3");
  add_ship(grid, 3, SHIP_2, 1, "D4"); 
  add_ship(grid, 4, SHIP_1, 0, "H6");
  add_ship(grid, 5, SHIP_4, 1, "B7");
  add_ship(grid, 6, SHIP_3, 1, "F7");
  add_ship(grid, 7, SHIP_2, 0, "H9"); 
}


void setup_pc_ships(Grid *grid) {
  ShipType ships_type[NUM_SHIPS] = {SHIP_1, SHIP_1, SHIP_3, SHIP_2, SHIP_1, SHIP_4, SHIP_3, SHIP_2};

  //from what i read we we can only call srand once in the program
  //so we use a static variable to ensure it is only called once
  static int seeded = 0;
  if (!seeded) { srand(time(NULL)); seeded = 1; }

  //we randomly place ships on the grid
  for (int ship_id = 0; ship_id < NUM_SHIPS; ship_id++) {
    int placed = 0;
    int orientation, row, col, size = SHIP_TYPE_TO_SIZE(ships_type[ship_id]);
    while (!placed) {
      orientation = rand() % 2; 
      if (orientation == 0) {  //horizontal
        row = rand() % GRID_ROWS;
        col = rand() % (GRID_COLS - size + 1);
      } else { //vertical
        row = rand() % (GRID_ROWS - size + 1);
        col = rand() % GRID_COLS;
      }
      if (can_place_ship(grid, row, col, size, orientation, ship_id)) {
        grid->ships[ship_id].type = ships_type[ship_id];
        grid->ships[ship_id].size = size;
        grid->ships[ship_id].orientation = orientation;
        grid->ships[ship_id].start_row = row;
        grid->ships[ship_id].start_col = col;
        grid->ships[ship_id].status = ALIVE;
        for (int i = 0; i < size; i++) {
          int r = (orientation == 0) ? row : row + i;
          int c = (orientation == 0) ? col + i : col;
          grid->cells[r][c].state = SHIP;
          grid->cells[r][c].ship_id = ship_id;
        }
        placed = 1;
      }
    }
  }
}


//we trace the available cells for the pc to bomb (avaliable cells: EMPTY or SHIP)
void pc_init_available_cells(Grid *grid) {
  pc_available_count = 0;
  for (int i = 0; i < GRID_ROWS; i++) {
    for (int j = 0; j < GRID_COLS; j++) {
      if (grid->cells[i][j].state != HIT && grid->cells[i][j].state != MISS) {

        pc_available_rows[pc_available_count] = i; //we store the index of the cells'row
        pc_available_cols[pc_available_count] = j; //we store the index of the cells'col

        /*whenever we call these lists on iindex 'pc_available_count' we will be given 
        the coordinates (row and col respectively) of a available cell*/
        pc_available_count++; 
      }
    }
  }
}





