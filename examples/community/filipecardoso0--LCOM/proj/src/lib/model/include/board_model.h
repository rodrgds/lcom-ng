#ifndef BOARD_MODEL_H
#define BOARD_MODEL_H

#include "snake_model.h"
#include "wall_model.h"
#include "fruit_model.h"


/**
 * @brief Structure that represents the game board. It contains the width and height coordinates, and the Pointers of structures that represent the various elements in the game (snake, walls and fruits).
 * 
 */
struct board {
  uint16_t width, height;
  snake_t* snake1, /**snake2,*/ snake1_dbuffer/*, snake2_dbuffer*/;
  wall_t** walls;
  fruit_t* fruit;
};

typedef struct board board_t;

/**
 * @brief Creates a new board with the specified with and height values.
 * 
 * @param width width value.
 * @param height height value.
 * @return board_t* Points to the board created.
 */
board_t* board_new(uint16_t width, uint16_t height);

/**
 * @brief Deletes a board.
 * 
 * @param board Points to the board to be removed.
 */
void board_delete(board_t** board);

/**
 * @brief Returns the board's width.
 * 
 * @param board Points to board that will retrieve its width.
 * @return uint16_t Board's width value.
 */
uint16_t board_get_width(board_t* board);

/**
 * @brief Returns the board's height.
 * 
 * @param board Points to board that will retrieve its height.
 * @return uint16_t Board's height value.
 */
uint16_t board_get_height(board_t* board);

// /**
//  * @brief Returns the snake according to its number.
//  * 
//  * @param board Points to the board that will retrieve the snake.
//  * @param n_snake Snake's number.
//  * @return snake_t* Points to the corresponding snake.
//  */
// snake_t* board_get_snake(board_t* board, int n_snake);

/**
 * @brief Returns the board's walls.
 * 
 * @param board Points to the board that will retrieve its walls.
 * @return wall_t** Points to the array that contains the board's walls.
 */
wall_t** board_get_walls(board_t* board);

/**
 * @brief Checks if a board does not have any element in a specified position.
 * 
 * @param board Points to the input board.
 * @param position Points to the position to be checked.
 * @return true If the board is empty in that position.
 * @return false Otherwise.
 */
bool board_is_empty(board_t* board, position_t* position);

#endif
