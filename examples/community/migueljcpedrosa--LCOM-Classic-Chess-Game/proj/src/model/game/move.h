/**
 * @file move.h
 * @brief This file contains the definition of the move module.
 * 
 * This includes the definition of the Move struct along with methods to create them.
 */

#ifndef _MOVE_H_
#define _MOVE_H_

#include "game_util.h"
#include <stdbool.h>

/**
 * @brief Represents a move.
 * 
 * This struct represents a move. It is composed of an origin, a destination, a type, a piece and a secondary piece.
 * 
 * The origin and destination are the positions of the move. \n
 * The type is the type of the move. \n
 * The destination is the position of the move. \n
 * The piece is the piece that is being moved. \n
 * The secondary piece is the piece that is being captured or castled with the move. \n
 * 
 * @see Position
 * @see MoveType
 * @see Piece
 */
typedef struct {
    Position origin;
    Position destination;
    MoveType type;

    struct Piece* piece;
    bool had_moved;
    Type old_type;

    struct Piece* secondary_piece;
} Move;

/**
 * @brief Creates a normal move.
 *  
 * @param origin The origin of the move
 * @param destination The destination of the move
 * @param piece The piece that is being moved
 * @param secondary_piece The piece that is being captured or castled with the move, can be NULL
 * @return Normal move
 */
Move create_normal_move(Position origin, Position destination, struct Piece* piece, struct Piece* secondary_piece);

/**
 * @brief Creates a capture move.
 *  
 * @param origin The origin of the move
 * @param destination The destination of the move
 * @param piece The piece that is being moved
 * @param secondary_piece The piece that is being captured or castled with the move, can be NULL
 * @return Promotion move
 */
Move create_capture(Position origin, Position destination, struct Piece* piece, struct Piece* secondary_piece);

/**
 * @brief Creates a promotion move.
 *  
 * @param origin The origin of the move
 * @param destination The destination of the move
 * @param piece The piece that is being moved
 * @param secondary_piece The piece that is being captured or castled with the move, can be NULL
 * @return Promotion move
 */
Move create_castle(Position origin, Position destination, struct Piece* piece, struct Piece* secondary_piece);

/**
 * @brief Creates a promotion move.
 *  
 * @param origin The origin of the move
 * @param destination The destination of the move
 * @param piece The piece that is being moved
 * @param secondary_piece The piece that is being captured or castled with the move, can be NULL
 * @return Promotion move
 */
Move create_en_passant(Position origin, Position destination, struct Piece* piece, struct Piece* secondary_piece);

#endif
