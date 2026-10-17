/**
 * This module contains all functions that handle the piece structure
 * Jorge Vale: 40%
 * Telmo Barros: 60%
 * Relative Weight: 9%
 */

#ifndef __PIECE_H
#define __PIECE_H

#include "sprite.h"

/** @defgroup Piece Piece
 * @{
 * Functions for manipulating piece
 */

#define FACE_LENGTH 		30

/** @name Piece structure */
/** @{
 *
 * Piece structure
 */
typedef struct{
	Sprite sprite; // sprite that composes piece
} Piece;
/** @} end of Piece structure */

/**
 * @brief Piece constructor
 *
 * @param x X coordinate of piece position
 * @param y Y coordinate of piece position
 *
 * @return A pointer to the piece
 */
Piece * new_piece(unsigned int x, unsigned int y);

/**
 * @brief Draws the piece in the video buffer
 *
 * @param piece Piece to draw
 */
void draw_piece(Piece * piece);

/**
 * @brief Deletes the piece by releasing the allocated memory
 *
 * @param piece Piece to destroy
 */
void delete_piece(Piece * piece);

/**@}*/

#endif /* __PIECE_H */
