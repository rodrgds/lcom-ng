/**
 * @file board
 * @author Diogo Pintado, Gabriel Machado Jr., José Costa, Linda Rodrigues
 * @brief This file contains the functions related to the game board and its pieces.
 * 
 * @date 2023-05-28 
 * 
 */

#ifndef _BOARD_H_
#define _BOARD_H_

#include <lcom/lcf.h>

#include "vbe_graphics.h"

#define SIZE 20 //square size
#define BOARD_MAX_IDX 199

#define UP_BREAK 0xc8 
#define LEFT_BREAK 0xcb 
#define RIGHT_BREAK 0xcd 
#define DOWN_MAKE 0x50 
#define ESC 0x81

/**
 * @brief Structure that represents a cell in the game board.
 * 
 * @param x The x position on board.
 * @param y The y position on board.
 * @param color The color of the cell.
 * 
 */

typedef struct {
  uint16_t x;               //position x on board
  uint16_t y;               //position y on board
  uint32_t color;
} Cell;



/**
 * @brief Structure that represents the game board.
 * 
 * @param cells Array of cells that represent the game board.
 * @param width The width of the board.
 * @param height The height of the board.
 * 
 */
typedef struct {
  Cell cells[200];
  uint16_t width;
  uint16_t height;
} Board;


/**
 * @brief Enum that represents the different shapes of the game pieces.
 * 
 */

enum shape{
  T,
  I,
  Z,
  O,
  L,
};


/**
 * @brief Structure that represents a game piece.
 * 
 * @param board_index Array of 4 indices referencing cells in the board array.
 * @param color Piece color.
 * @param rot Piece rotation value.
 * @param board Reference to the board the piece belongs to.
 * @param piece_shape Shape of the piece.
 * 
 */
typedef struct {
  uint16_t board_index[4];
  uint32_t color;
  int rot;             //1 to 4
  Board *board;
  enum shape piece_shape;
} Piece;

/**
 * @brief Function that draws the game board.
 * 
 * The function draws the borders of board and each of its cells with its respective color.
 * 
 * @param board The game board to be drawn.
 */
void draw_board(Board* board);

/**
 * @brief Function that creates a new game piece.
 * 
 * The function allocates memory for the new piece, selects a shape and a color for it, initializes its poisition and rotation
 * value and updates the board cells with the piece's color
 * 
 * @param board A pointer to the board the new piece belongs to.
 * @return Piece* - a pointer to the new game piece.
 */
Piece* new_piece(Board* board);

/**
 * @brief Function that creates a new game board.
 * 
 * The function allocates memory for the new board and intializes each of its cells.
 * 
 * @return Board* - a pointer to the new board.
 */
Board* new_board();

/**
 * @brief Function that frees the memory used by a game piece.
 * 
 * @param piece A pointer to the game piece to be deleted.
 */
void delete_piece(Piece* piece);

/**
 * @brief Function that frees the memory used by a game board.
 * 
 * @param board A pointer to the game board to be deleted.
 */
void delete_board(Board *board);

#endif
