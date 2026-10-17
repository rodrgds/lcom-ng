#include <minix/sysutil.h>
#include <lcom/lcf.h>
#include "sprite.h"
#ifndef SHARED_GAME_H
#define SHARED_GAME_H

/**
 * @brief Represents the state of a block in a Minesweeper game.
 *
 * This enumeration defines the possible states of a block in a Minesweeper game.
 * Blocks can be flagged, revealed, or not revealed.
 */

typedef enum{
    Flagged,
    Revealed,
    Not_Revealed
}BlockState;

/**
 * @brief Represents the type of content in a block in a Minesweeper game.
 *
 * This enumeration defines the possible types of content in a block in a Minesweeper game.
 * Blocks can contain numbers indicating adjacent bombs, a bomb itself, or be empty.
 */

typedef enum{
    ONE,
    TWO,
    THREE,
    FOUR,
    FIVE,
    SIX,
    SEVEN,
    EIGHT,
    BOMB,
    EMPTY
}BlockType;

/**
 * @brief Represents a grid entry.
 *
 * This structure defines the coordinates of a grid entry in a Minesweeper game.
 */

typedef struct {
    int16_t x,y;
} Grid_entry;

/**
 * @brief Represents a block in a Minesweeper game.
 *
 * This structure defines the properties of a block in a Minesweeper game,
 * including its sprite representations, type, position, selection status, and state.
 */

typedef struct {
    Sprite *block_sprite;
    Sprite *selected_block_sprite;
    BlockType type;
    int16_t row,col;
    bool is_selected;
    BlockState state;
} Block;

/**
 * @brief Places bombs randomly on the grid.
 * 
 * Places a specified number of bombs on the grid at random positions
 * ensuring that bombs are not placed at the initial entry point.
 */

void placeBombs();

/**
 * @brief Counts the number of bombs around a given cell.
 * 
 * Updates the block type and sprite of the cell based on the number
 * of surrounding bombs.
 * 
 * @param x The row position of the cell.
 * @param y The column position of the cell.
 */

void count_bombs_around(int16_t x,int16_t y);

/**
 * @brief Creates a new block.
 * 
 * Allocates memory for a new block and initializes its properties.
 * 
 * @param block_sprite Pointer to the sprite representing the block.
 * @param selected_block_sprite Pointer to the sprite representing the selected block.
 * @param type The type of the block.
 * @param row The row position of the block in the grid.
 * @param col The column position of the block in the grid.
 * @param is_selected Indicates if the block is selected.
 * @param state The state of the block (revealed, not revealed, flagged).
 * @return Pointer to the created block.
 */

Block *create_block(Sprite *block_sprite,Sprite *selected_block_sprite,BlockType type, int16_t row, int16_t col,bool is_selected,BlockState state);

/**
 * @brief Creates the game grid.
 * 
 * Allocates memory for the grid and initializes each cell with default values.
 */

void create_game();

/**
 * @brief Destroys the game grid.
 * 
 * Frees the allocated memory for the grid and its cells.
 */

void destroy_game();

/**
 * @brief Checks if the player has won the game.
 * 
 * Returns 1 if the player has revealed all non-bomb cells, otherwise 0.
 * 
 * @return int 1 if the player has won, 0 otherwise.
 */

int check_win();

/**
 * @brief Counts the number of flagged cells.
 * 
 * Updates the flag count based on the number of flagged cells in the grid.
 * 
 * @param flag_count Pointer to the flag count variable to update.
 */

void flag_counter(uint8_t *flag_count);

/**
 * @brief Checks if more flags can be placed.
 * 
 * This function counts the current number of flagged cells and checks if it
 * is less than the allowed maximum (10 flags). If the number of flags is 
 * less than 10, more flags can be placed.
 * 
 * @return int 1 if more flags can be placed, 0 otherwise.
 */

int check_can_flag();

/**
 * @brief Checks if the first cell has been touched.
 * 
 * This function iterates through the grid to check if any cell has been revealed.
 * If no cell is revealed, it indicates the first touch has not been made.
 * 
 * @return int 1 if no cell has been revealed, 0 otherwise.
 */

int check_first_touch();

/**
 * @brief Reveals cells near a cell with zero bombs around it.
 * 
 * This function reveals all cells in the vicinity of a given cell that has
 * zero bombs around it. If any of the neighboring cells are also zero, it 
 * recursively reveals their neighbors.
 * 
 * @param x The row position of the cell.
 * @param y The column position of the cell.
 */

void reveal_near_zeros(int16_t x , int16_t y);
#endif //SHARED_GAME_H
