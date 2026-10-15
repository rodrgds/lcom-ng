/** 
 * @file board.h
 * 
 * @brief This file contains the definition of the board module.
 * 
 * This includes the definition of the Board struct, along with
 * methods for it.
 * 
 * @see Board
*/

#ifndef _BOARD_H_
#define _BOARD_H_

#include "pieces.h"
#include "player.h"

extern unsigned int board_start;
extern unsigned int square_size;

/**
 * @brief Represents a chess board.
 * 
 * This struct represents a chess board. It is composed of an array of 64 squares.
 * Each square can be empty or contain a piece.
 * 
 * @see Piece
 */
typedef struct {
    Piece* squares[64];
} Board;

/**
 * @brief Initializes a board and it's pieces.
 * 
 * This function creates the board and places the pieces in their initial positions.
 * It also associates the pieces with their respective players.
 * 
 * @param white_player pointer to the player that will control the white pieces.
 * @param black_player pointer to the player that will control the black pieces.
 * 
 * @return Returns the created board.
 */
Board* (create_board)(Player* white_player, Player* black_player);

/**
 * @brief Sets the black pieces in their initial positions.
 * 
 * This function sets the black pieces in their initial positions 
 * and associates each with the black player.
 * 
 * @param board pointer to the board where the pieces will be placed.
 * @param black_player pointer to the player that will control the black pieces.
 */
void (set_black_pieces)(Board* board, Player* black_player);


/** 
 * @brief Sets the white pieces in their initial positions.
 * 
 * This function sets the white pieces in their initial positions
 * and associates each with the white player.
 * 
 * @param board pointer to the board where the pieces will be placed.
 * @param white_player pointer to the player that will control the white pieces.
*/
void (set_white_pieces)(Board* board, Player* white_player);

/**
 * @brief Destroys a board.
 * 
 * This function destroys a board and all of it's pieces.
 * 
 * @param board pointer to the board to be destroyed.
 */
void (destroy_board)(Board* board);

#endif
