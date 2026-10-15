#ifndef GAME_MACRO_H
#define GAME_MACRO_H

/// @brief Target game update frequency in Hz (used with timer)
#define FREQUENCY 60

/// @brief Timer number used for configuration
#define TIMER 0

/// @brief Video mode used for graphics
#define VIDEO_MODE 0x115

// ---------------- Grid Definitions ----------------

/// @brief Number of rows in the game grid
#define GRID_ROWS 10

/// @brief Number of columns in the game grid
#define GRID_COLS 10

/// @brief Pixel height of the entire grid
#define GRID_HEIGHT 378

/// @brief Pixel width of the entire grid
#define GRID_WIDTH 396

/// @brief Pixel width of a single cell in the grid
#define CELL_WIDTH 33

/// @brief Pixel height of a single cell in the grid
#define CELL_HEIGHT 33

/// @brief Total number of ships in the game
#define NUM_SHIPS 8

/**
 * @brief Converts a ShipType enum to its size.
 * 
 * @param type The ShipType to evaluate.
 * @return The size of the ship, or 0 if NO_SHIP.
 */
#define SHIP_TYPE_TO_SIZE(type) ((type == NO_SHIP) ? 0 : (int)type)

// ---------------- Key Mappings ----------------

#define ESC_KEY 0x81
#define Q_KEY 0x10
#define SPACE_KEY 0x39
#define M_KEY 0x32
#define ENTER_KEY 0x1C
#define R_KEY 0x13
#define H_KEY 0x23
#define S_KEY 0x1F
#define UP_ARROW_KEY 0x48
#define DOWN_ARROW_KEY 0x50
#define ONE_KEY 0x82
#define TWO_KEY 0x83

// ---------------- Color Definitions (Hex RGB) ----------------

#define BLACK 0x000000
#define RED 0xFF0000
#define PINK 0xFF00FF
#define BLUE 0x0000FF
#define BETTER_BLUE 0x4f80a8
#define YELLOW 0xFFFF00
#define PURPLE 0x800080
#define HOVER_COLOR 0xAAAAFF
#define SHIP_COLOR 0x1a2630
#define SHIP_HOVER_COLOR 0xdb67b8
#define ORANGE 0xFFA500
#define DARK_ORANGE 0xb57a3e
#define BRIGHT_ORANGE 0xf09b13
#define NAVY_BLUE 0x293f87
#define TEAL 0x297487
#define WHITE 0xFFFFFF
#define CREAM 0xeec8ce
#define LILAC 0xA27EBA

// ---------------- Type Definitions ----------------

/**
 * @enum ShipType
 * @brief Types of ships in the game.
 */
typedef enum {
    NO_SHIP, ///< No ship
    SHIP_1,  ///< Smallest ship
    SHIP_2,  ///< Medium ship
    SHIP_3,  ///< Large ship
    SHIP_4,  ///< Largest ship
} ShipType;

/**
 * @enum ShipStatus
 * @brief The status of a ship.
 */
typedef enum {
    ALIVE, ///< The ship is still afloat
    SUNK   ///< The ship has been destroyed
} ShipStatus;

/**
 * @struct Ship
 * @brief Represents a ship in the game.
 */
typedef struct {
    ShipType type;       ///< The ship's type
    int size;            ///< The ship's size (number of cells)
    int orientation;     ///< 0 for horizontal, 1 for vertical
    int start_row;       ///< Row where the ship starts
    int start_col;       ///< Column where the ship starts
    ShipStatus status;   ///< Current status of the ship
} Ship;

/**
 * @enum CellState
 * @brief The state of a grid cell.
 */
typedef enum {
    EMPTY, ///< No ship and not attacked
    SHIP,  ///< Ship present
    HIT,   ///< Ship was hit
    MISS   ///< Attack missed
} CellState;

/**
 * @struct Cell
 * @brief Represents a cell in the grid.
 */
typedef struct {
    CellState state; ///< Current state of the cell
    int ship_id;     ///< Index of the ship occupying the cell, -1 if none
} Cell;

/**
 * @struct Grid
 * @brief Represents a player's grid.
 */
typedef struct {
    Cell cells[GRID_ROWS][GRID_COLS]; ///< 2D array of grid cells
    Ship ships[NUM_SHIPS];            ///< Array of ships
    int ships_remaining;              ///< Ships left (0 = defeated)
    int sprite_x, sprite_y;           ///< Position of the grid sprite
    int x, y;                         ///< Coordinates of the top-left cell
} Grid;

/**
 * @struct Arena
 * @brief Represents the full game arena with both players' grids.
 */
typedef struct {
    Grid player1_grid; ///< Grid for player 1
    Grid player2_grid; ///< Grid for player 2
} Arena;

/**
 * @enum ArenaPhase
 * @brief Current phase of the game.
 */
typedef enum {
  SETUP_PLAYER1, ///< Player 1 is placing ships
  SETUP_PLAYER2, ///< Player 2 is placing ships
  BATTLE_PHASE   ///< Active battle between players
} ArenaPhase;

/**
 * @enum PlayerTurn
 * @brief Indicates which player's turn it is.
 */
typedef enum {
  PLAYER_1, ///< Player 1's turn
  PLAYER_2  ///< Player 2's turn
} PlayerTurn;

/**
 * @enum GameMode
 * @brief Game mode selected by the player(s).
 */
typedef enum {
  SINGLE_PLAYER, ///< Playing against AI
  MULTI_PLAYER,  ///< Two-player mode
} GameMode;

#endif // GAME_MACRO_H
