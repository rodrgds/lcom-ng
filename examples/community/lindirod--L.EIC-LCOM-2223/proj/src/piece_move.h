/**
 * @file piece_move
 * @author Diogo Pintado, Gabriel Machado Jr., José Costa, Linda Rodrigues
 * 
 * @brief Contains functions used for moving a Tetris piece on the board.
 * 
 */
#ifndef _PIECE_MOVE_H_
#define _PIECE_MOVE_H_

#include "board.h"

/**
 * @brief Enum containing the different types of movement for a Tetris piece
 * 
 */
enum movement_type{
  ROTATE,     ///< Rotate the piece
  DOWN,       ///< Move the piece downwards 
  LEFT,       ///< Move the piece to the left
  RIGHT,      ///< Move the piece to the right
};

/**
 * @brief Moves the Tetris piece to the right
 * 
 * @param piece Pointer to the Tetris piece to be moved
 * @return true if the piece was moved successfully, false otherwise
 */
bool move_right(Piece * piece);

/**
 * @brief Moves the Tetris piece to the left
 * 
 * @param piece Pointer to the Tetris piece to be moved
 * @return true  if the piece was moved successfully, false otherwise
 */
bool move_left(Piece * piece);

/**
 * @brief Rotates the Tetris piece
 * 
 * @param piece Pointer to the Tetris piece to be rotated
 * @return true if the piece was rotated successfully, false otherwise
 */
bool rotate_piece(Piece * piece);

/**
 * @brief Updates the movement of the Tetris piece
 * 
 * @param piece Pointer to the Tetris piece to be moved
 * @param move Type of movement to be performed
 * @return true if the piece was moved successfully, false otherwise
 */
bool update_movement(Piece * piece, enum movement_type move);

/**
 * @brief Moves the Tetris piece downwards
 * 
 * @param piece Pointer to the Tetris piece to be moved
 * @return true if the piece was moved successfully, false otherwise
 */
bool move_down(Piece * piece);

#endif
