#ifndef __PROJ_BOARD_H
#define __PROJ_BOARD_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/** @defgroup board board
 * @{
 *
 * Functions for parsing game board from text files
 */

#define MAX_BOARD_WIDTH 17                    /**< @brief Maximum width of the game board */
#define MAX_BOARD_HEIGHT 11                   /**< @brief Maximum height of the game board */
#define MAX_LINE_LENGTH (MAX_BOARD_WIDTH + 2) /**< @brief Maximum line length for reading board files */

/**
 * @brief Enum representing different types of board elements
 */
typedef enum {
  PLAYER,     /**< @brief Player starting position */
  ENEMY,      /**< @brief Enemy starting position */
  BRICK,      /**< @brief Destructible brick */
  WALL,       /**< @brief Indestructible wall */
  BOMB,       /**< @brief Bomb */
  POWERUP,    /**< @brief Power-up item */
  DOOR,       /**< @brief Door to pass to the next level */
  EMPTY_SPACE /**< @brief Empty space where players can move */
} board_element_t;

/**
 * @brief Struct representing the game board
 *
 * Uses fixed-size arrays for board elements to avoid dynamic memory allocation.
 */
typedef struct {
  uint8_t width;                                               /**< @brief Actual width of the board in cells */
  uint8_t height;                                              /**< @brief Actual height of the board in cells */
  board_element_t elements[MAX_BOARD_HEIGHT][MAX_BOARD_WIDTH]; /**< @brief Fixed-size 2D array of board elements */
} GameBoard;

/**
 * @brief Loads a game board from a text file
 *
 * Reads the board layout from a text file and initializes the GameBoard structure.
 * The board is stored in a fixed-size array to avoid dynamic memory allocation.
 *
 * @param board Pointer to the GameBoard structure to fill
 * @param filename Path to the text file containing the board layout
 *
 * @return 0 upon success, non-zero otherwise
 */
int(load_board)(GameBoard *board, const char *filename);

/**
 * @brief Resets a game board to its initial empty state
 *
 * Clears all cells in the board's elements array and resets dimensions.
 *
 * @param board Pointer to the board to be reset
 *
 * @return 0 upon success, non-zero otherwise
 */
int(reset_board)(GameBoard *board);

/**@}*/

#endif /* __PROJ_BOARD_H */
