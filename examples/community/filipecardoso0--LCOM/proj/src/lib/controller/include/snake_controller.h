#ifndef SNAKE_CONTROLLER_H
#define SNAKE_CONTROLLER_H

#include "../../state/include/snake_state.h"
#include "../../model/include/board_model.h"
#include "../../state/include/action.h"

/**
 * @brief Makes a snake's step in the board according to the input action.
 * 
 * @param snake Points to the snake to be updated according the input action.
 * @param board Points to the board that contains the snake and will update the snake's details.
 * @param action Input action.
 */
int snake_step(snake_t* snake, board_t* board, action_t action);

/**
 * @brief Moves a snake in the board.
 * 
 * @param snake Points to the snake to be moved.
 * @param board Points to the board that contains the snake and will update the snake's position. 
 */
int snake_move(snake_t* snake, board_t* board);

/**
 * @brief Moves a snake to the left in the board.
 * 
 * @param snake Points to the snake to be moved.
 * @param board Points to the board that contains the snake and will update the snake's position. 
 */
int snake_move_left(snake_t* snake, board_t* board);

/**
 * @brief Moves a snake to the right in the board.
 * 
 * @param snake Points to the snake to be moved.
 * @param board Points to the board that contains the snake and will update the snake's position. 
 */
int snake_move_right(snake_t* snake, board_t* board);

/**
 * @brief Moves a snake up in the board.
 * 
 * @param snake Points to the snake to be moved.
 * @param board Points to the board that contains the snake and will update the snake's position. 
 */
int snake_move_up(snake_t* snake, board_t* board);

/**
 * @brief Moves a snake down in the board.
 * 
 * @param snake Points to the snake to be moved.
 * @param board Points to the board that contains the snake and will update the snake's position. 
 */
int snake_move_down(snake_t* snake, board_t* board);

#endif
