/** 
 * @file pieces.h
 * @brief This file contains the definition of the piece module.
 * 
 * This includes the definition of the Piece struct, along with
 * methods for them.
 * 
 * @see Piece
*/

#ifndef _PIECE_H_
#define _PIECE_H_

#include <stdbool.h>
#include "../../view/sprite.h"
#include "game_util.h"
#include "move.h"

/**
 * @brief Represents a chess piece.
 * 
 * This struct represents a chess piece. It is composed of a position, a color, a type, a status and a sprite. 
 * 
 * @see Position
 * @see Sprite
 * @see Type
 * @see Color
 * @see Status 
 */
typedef struct Piece {

    Position position;

    Color color;
    Type type;
    bool has_moved;
    enum {ALIVE, CAPTURED} status;
    
    Move* moves;
    int num_moves;

    Sprite* sprite;
} Piece;

/**
 * @brief Creates a position.
 * 
 * This function creates a position with the given board index.
 * 
 * @param boardIndex the index of the position in the board.
 * 
 * @return Returns the created position.
 */
Position (create_position)(int boardIndex);

/** 
 * @brief Creates a piece.
 * 
 * This function creates a piece with the given type, color and position.
 * 
 * @param type the type of the piece.
 * @param color the color of the piece.
 * @param pos the position of the piece.
 * @return Returns the created piece.
*/
Piece* (create_piece)(Type type, Color color, Position pos);

#endif
