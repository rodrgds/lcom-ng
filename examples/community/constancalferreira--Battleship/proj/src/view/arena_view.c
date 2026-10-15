#include "arena_view.h"

extern uint8_t *current_buffer;
extern uint8_t *arena_buffer;
extern unsigned int frame_size;
extern vbe_mode_info_t mode_info;

extern Sprite *ship1;
extern Sprite *ship2h;
extern Sprite *ship2v;
extern Sprite *ship3h;
extern Sprite *ship3v;
extern Sprite *ship4h;
extern Sprite *ship4v;
extern Sprite *double_grid;
extern Sprite *single_grid;
extern Sprite *player1;
extern Sprite *player2;
extern Sprite *setup;

extern Arena arena;
extern ArenaPhase arena_phase;
extern PlayerTurn current_player;

extern int cursor_x;
extern int cursor_y;


//MAKE arena.player1_grid A GLOBAL VARIABLE !!!!!!!!


void draw_arena_background(uint8_t *buffer) {
  fill_screen(BETTER_BLUE, buffer);
}



void draw_arena() {
  memcpy(current_buffer, arena_buffer, frame_size);

  draw_layout();


  //NEEDS FIXING
  if (arena_phase == SETUP_PLAYER1 && current_player == PLAYER_1) {
    draw_grid(&arena.player1_grid, 0);
    draw_sprite(setup, arena.player2_grid.sprite_x + 20, arena.player2_grid.sprite_y + 10, current_buffer);
  } else if (arena_phase == SETUP_PLAYER2 && current_player == PLAYER_2) {
    draw_grid(&arena.player2_grid, 0);
    draw_sprite(setup, arena.player1_grid.sprite_x + 20, arena.player1_grid.sprite_y + 10, current_buffer);
  
  } else if (arena_phase == BATTLE_PHASE) {
    if (current_player == PLAYER_1) {
      draw_grid(&arena.player1_grid, 1);
      draw_grid(&arena.player2_grid, 1);
    } else if (current_player == PLAYER_2) {
      draw_grid(&arena.player2_grid, 1);
      draw_grid(&arena.player1_grid, 1);
    }  
  }

  draw_player(current_player);
  if (arena_phase == BATTLE_PHASE) draw_guide_ships();
}


void draw_grid(Grid *grid, bool hide_ships) {
  int hovered_row, hovered_col, hovered_ship_id;
  bool hovering = mouse_over_ship(grid, cursor_x, cursor_y, &hovered_row, &hovered_col, &hovered_ship_id);
  
  for (int i = 0; i < GRID_ROWS; i++) {
    for (int j = 0; j < GRID_COLS; j++) {
      int x, y;
      cell_to_pixel(grid, i, j, &x, &y);
      draw_cell(grid, x, y, i, j, hovering ? hovered_ship_id : -1, hide_ships);
    }
  }
}


void draw_layout() {
  
  if (arena_phase == SETUP_PLAYER1) {
    draw_sprite_recolor(single_grid, arena.player1_grid.sprite_x, arena.player1_grid.sprite_y, BRIGHT_ORANGE, current_buffer);
  } else if (arena_phase == SETUP_PLAYER2) {
    draw_sprite_recolor(single_grid, arena.player2_grid.sprite_x, arena.player2_grid.sprite_y, BRIGHT_ORANGE, current_buffer);
  }

  if (arena_phase == BATTLE_PHASE) {
    if (current_player == PLAYER_1) {
      draw_sprite(single_grid, arena.player1_grid.sprite_x, arena.player1_grid.sprite_y, current_buffer);
      draw_sprite_recolor(single_grid, arena.player2_grid.sprite_x, arena.player2_grid.sprite_y, BRIGHT_ORANGE, current_buffer);
    } else if (current_player == PLAYER_2) {
      draw_sprite_recolor(single_grid, arena.player1_grid.sprite_x, arena.player1_grid.sprite_y, BRIGHT_ORANGE, current_buffer);
      draw_sprite(single_grid, arena.player2_grid.sprite_x, arena.player2_grid.sprite_y, current_buffer);
    }
  }
}


void draw_cell(Grid *grid, int x, int y, int cell_row, int cell_col, int hovered_ship_id, bool hide_ships) {

  Cell *cell = &grid->cells[cell_row][cell_col];
  Ship *ship = &grid->ships[cell->ship_id];

  //only draw ships at their starting position to avoid duplicates
  if ((cell->state == SHIP || cell->ship_id >= 0) && !hide_ships) {
    if (ship->start_row == cell_row && ship->start_col == cell_col) {
        draw_ship_sprite(x, y, ship->type, ship->orientation, 0);
    }
  }

  //always show cell hover
  if (cursor_x >= x && cursor_x < x + CELL_WIDTH &&
      cursor_y >= y && cursor_y < y + CELL_HEIGHT) {
     if (!((arena_phase != BATTLE_PHASE && cell->ship_id > 0) || (cell->ship_id >= 0 && ship->status == SUNK)))  //dont want to draw hoover on ships in SETUP_PHASE's
      draw_rectangle(x, y, CELL_WIDTH, CELL_HEIGHT, HOVER_COLOR, current_buffer);
  }


  //only show ship hover in SETUP_PHASE
  if ((arena_phase == SETUP_PLAYER1 || arena_phase == SETUP_PLAYER2) && cell->ship_id == hovered_ship_id && hovered_ship_id >= 0) {
    if(ship->start_row == cell_row && ship->start_col == cell_col) {
      draw_ship_sprite(x, y, ship->type, ship->orientation, 1);
    }
  }


  if (cell->state == HIT) {
    if (ship->status == ALIVE) draw_hit_marker(x, y); 
    else if (ship->status == SUNK && ship->start_row == cell_row && ship->start_col == cell_col) {
      draw_ship_sprite(x, y, ship->type, ship->orientation, 0);
    }
  }
  if (cell->state == MISS) {
    draw_miss_marker(x, y);
  }
}



//the next 2 functions could be together but i wanted to keep the code clean
//they draw the ship status of each player in the bottom of the screen

void draw_ship_status(Grid *grid, int start_x, int start_y) {
  int x_offset = 0;
  int y_offset = 0;

  for (int i = 0; i < NUM_SHIPS; i++) {
    Ship *ship = &grid->ships[i];
    int size = SHIP_TYPE_TO_SIZE(ship->type);
    int ship_width = 10*size;  
    int ship_height = 10; 
    uint32_t color = (ship->status == ALIVE) ? NAVY_BLUE : ORANGE; 

    draw_rectangle(start_x + x_offset, start_y + y_offset, ship_width, ship_height, color, current_buffer);

    x_offset += ship_width + 10; 
  }
}

void draw_guide_ships() {
  int player1_x = arena.player1_grid.sprite_x + 85;
  int player2_x = arena.player2_grid.sprite_x + 85;
  int player_y = arena.player1_grid.sprite_y + GRID_HEIGHT + 90;

  draw_ship_status(&arena.player1_grid, player2_x, player_y); //makes more sense to draw the ships i sunk bellow me doenst it? i think so
  draw_ship_status(&arena.player2_grid, player1_x, player_y);
}




void draw_ship_sprite(int x, int y, ShipType type, int orientation, bool hoover) {
  Sprite *sprite = NULL;
  switch (type) {
    case SHIP_1: sprite = ship1; break;
    case SHIP_2: sprite = (orientation == 0) ? ship2h : ship2v; break;
    case SHIP_3: sprite = (orientation == 0) ? ship3h : ship3v; break;
    case SHIP_4: sprite = (orientation == 0) ? ship4h : ship4v; break;
    default: return;
  }
  if (hoover) draw_sprite_recolor(sprite, x, y, SHIP_HOVER_COLOR, current_buffer);
  else draw_sprite(sprite, x, y, current_buffer);
}



void draw_hit_marker(int x, int y) {
  draw_rectangle(x, y, CELL_WIDTH, CELL_HEIGHT, RED, current_buffer);
}

void draw_miss_marker(int x, int y) {
  draw_rectangle(x, y, CELL_WIDTH, CELL_HEIGHT, TEAL, current_buffer);
}


void draw_player(PlayerTurn player) {

  int player1_x = arena.player1_grid.sprite_x + GRID_WIDTH/2 - player1->width/2;
  int player2_x = arena.player2_grid.sprite_x + GRID_WIDTH/2 - player2->width/2;
  int player_y = arena.player1_grid.sprite_x + GRID_HEIGHT + 50;

  if (arena_phase == SETUP_PLAYER1) {
    draw_sprite_recolor(player1, player1_x, player_y, BRIGHT_ORANGE, current_buffer);
  } else if (arena_phase == SETUP_PLAYER2) {
    draw_sprite_recolor(player2, player2_x, player_y, BRIGHT_ORANGE, current_buffer);
  } else if (BATTLE_PHASE) {
    if (player == PLAYER_1) {
      draw_sprite_recolor(player1, player1_x, player_y, BRIGHT_ORANGE, current_buffer);
      draw_sprite(player2, player2_x, player_y, current_buffer);
    } else if (player == PLAYER_2) {
      draw_sprite_recolor(player2, player2_x, player_y, BRIGHT_ORANGE, current_buffer);
      draw_sprite(player1, player1_x, player_y, current_buffer);
    }
  }
}

