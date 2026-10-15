/**
 * @file arena.h
 * @brief Arena logic and interface for the battleship game.
 *
 * Contains functions and structures for managing the arena, player grids,
 * ship placement, battle phase, and mouse/keyboard interactions.
 */

#ifndef ARENA_H
#define ARENA_H


#include <lcom/lcf.h>
#include <minix/sysutil.h>
#include <ctype.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "controller/graphics.h"
#include "model/game_macro.h"
#include "model/game.h"
#include "view/game_view.h"
#include "view/arena_view.h"

/// Arena buffer for double buffering
extern uint8_t *arena_buffer;

/// Current phase of the arena (setup, battle, etc.)
extern ArenaPhase arena_phase;

/// Main arena structure containing both player grids
extern Arena arena;

/// Indicates which player's turn it is
extern PlayerTurn current_player;

/**
 * @struct DragState
 * @brief Structure to handle ship dragging during setup phase.
 */
typedef struct {
    bool dragging;           /**< Is a ship currently being dragged? */
    int ship_id;             /**< ID of the ship being dragged */
    int origin_row;          /**< Row where the drag started */
    int origin_col;          /**< Column where the drag started */
    int offset;              /**< Offset from the ship's first cell to the cell where the mouse is */
    Grid *active_grid;       /**< Grid where the ship is being dragged */
} DragState;

/// Global drag state for ship dragging
extern DragState drag_state;

/**
 * @brief Main loop for the arena screen.
 */
void arena_main_loop();

/**
 * @brief Allocates the arena buffer if not already allocated.
 */
void set_arena_buffer();

/**
 * @brief Initializes the arena, grids, ships, and background.
 */
void init_arena();

/**
 * @brief Initializes a grid's position and state.
 * @param grid Pointer to the grid to initialize
 * @param sprite_x X coordinate for the grid sprite
 * @param sprite_y Y coordinate for the grid sprite
 */
void init_grid(Grid *grid, int sprite_x, int sprite_y);

/**
 * @brief Handles keyboard input in the arena screen.
 */
void arena_keyboard_handler();

/**
 * @brief Handles mouse input in the arena screen.
 */
void arena_mouse_handler();

/**
 * @brief Handles a mouse click on a grid during the battle phase.
 * @param grid Pointer to the grid
 * @param mouse_x Mouse X coordinate
 * @param mouse_y Mouse Y coordinate
 * @return true if a valid cell was clicked, false otherwise
 */
bool handle_mouse_click(Grid *grid, int mouse_x, int mouse_y);

/**
 * @brief Handles mouse input during the battle phase.
 * @param curr_lb Current left button state
 * @param prev_lb Previous left button state
 */
void battle_phase_mouse(bool curr_lb, bool prev_lb);

/**
 * @brief Handles the PC's turn during the battle phase (single player).
 */
void battle_phase_pc();

/**
 * @brief Handles mouse input during the setup phase.
 * @param curr_lb Current left button state
 * @param prev_lb Previous left button state
 * @param curr_rb Current right button state
 * @param prev_rb Previous right button state
 * @param grid Pointer to the grid being set up
 */
void setup_phase(bool curr_lb, bool prev_lb, bool curr_rb, bool prev_rb, Grid *grid);

/**
 * @brief Checks if the mouse is over a ship.
 * @param grid Pointer to the grid
 * @param mouse_x Mouse X coordinate
 * @param mouse_y Mouse Y coordinate
 * @param row Output: row of the ship cell
 * @param col Output: column of the ship cell
 * @param ship_id Output: ID of the ship
 * @return true if mouse is over a ship, false otherwise
 */
bool mouse_over_ship(Grid *grid, int mouse_x, int mouse_y, int *row, int *col, int *ship_id);

/**
 * @brief Checks if the mouse is over a grid cell.
 * @param grid Pointer to the grid
 * @param mouse_x Mouse X coordinate
 * @param mouse_y Mouse Y coordinate
 * @param row Output: row of the cell
 * @param col Output: column of the cell
 * @return true if mouse is over a cell, false otherwise
 */
bool mouse_over_cell(Grid *grid, int mouse_x, int mouse_y, int *row, int *col);

/**
 * @brief Moves a ship to a new position on the grid.
 * @param grid Pointer to the grid
 * @param ship_id ID of the ship to move
 * @param new_row New starting row
 * @param new_col New starting column
 */
void move_ship(Grid *grid, int ship_id, int new_row, int new_col);

/**
 * @brief Checks if a ship is sunk.
 * @param grid Pointer to the grid
 * @param ship_id ID of the ship
 * @return true if the ship is sunk, false otherwise
 */
bool is_ship_sunk(Grid *grid, int ship_id);

/**
 * @brief Converts a grid cell to pixel coordinates.
 * @param grid Pointer to the grid
 * @param row Row index
 * @param col Column index
 * @param x Output: X pixel coordinate
 * @param y Output: Y pixel coordinate
 */
void cell_to_pixel(Grid *grid, int row, int col, int* x, int* y);

/**
 * @brief Converts a coordinate string (e.g., "A1") to cell indices.
 * @param coord Coordinate string
 * @param row Output: row index
 * @param col Output: column index
 * @return 0 on success, 1 on error
 */
int coord_to_cell(const char* coord, int* row, int* col);

/**
 * @brief Checks if a ship can be placed at a given position.
 * @param grid Pointer to the grid
 * @param start_row Starting row
 * @param start_col Starting column
 * @param size Size of the ship
 * @param orientation Orientation (0: horizontal, 1: vertical)
 * @param ship_id ID of the ship
 * @return true if the ship can be placed, false otherwise
 */
bool can_place_ship(Grid *grid, int start_row, int start_col, int size, int orientation, int ship_id);

/**
 * @brief Adds a ship to the grid.
 * @param grid Pointer to the grid
 * @param ship_id ID of the ship
 * @param type Ship type
 * @param orientation Orientation (0: horizontal, 1: vertical)
 * @param coord Coordinate string (e.g., "A1")
 * @return true on success, false on error
 */
bool add_ship(Grid *grid, int ship_id, ShipType type, int orientation, const char* coord);

/**
 * @brief Sets up all ships for a player grid (default positions).
 * @param grid Pointer to the grid
 */
void setup_ships(Grid *grid);

/**
 * @brief Sets up all ships for the PC grid (random positions).
 * @param grid Pointer to the grid
 */
void setup_pc_ships(Grid *grid);

/**
 * @brief Initializes the list of available cells for the PC to bomb.
 * @param grid Pointer to the grid
 */
void pc_init_available_cells(Grid *grid);


#endif
