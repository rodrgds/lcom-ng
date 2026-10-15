/**
 * @file game_util.h
 * @brief This file contains the definition of the game util module.
 * 
 * This includes the definition of the Position, Color, Type and MoveType structs along with other useful constants.
 * 
 * @see Position
 * @see Color
 * @see Type
 * @see MoveType
 */

#ifndef _GAME_UTIL_H_
#define _GAME_UTIL_H_

/** 
 * @brief Defines the default round time in seconds.
*/
#define ROUND_TIME 600

/**
 * @brief Represents a position.
 * 
 * This struct represents a position. It is composed of an x and a y coordinate.
 * 
 * @see Piece
 * @see Move
 */
typedef struct
{
    int x;
    int y;
} Position;

/** 
 * @brief Represents a chess color.
 * 
 * This enum represents a chess color. It can be either white or black.
 * 
 * @see Piece
*/
typedef enum {
    WHITE,
    BLACK,
} Color;

/**
 * @brief Represents a chess piece type.
 * 
 * This enum represents a chess piece type. It can be either a pawn, a knight, a rook, a bishop, a queen or a king.
 * 
 * @see Piece
 */
typedef enum {
    PAWN,
    KNIGHT,
    ROOK,
    BISHOP,
    QUEEN,
    KING
} Type;

/** 
 * @brief Represents a move type.
 * 
 * This enum represents a move type. 
 * It can be either be: \n
 * - Normal: a simple movimentation in the board. \n
 * - Capture: a movimentation that captures a piece. \n
 * - Castle: a movimentation that castles the king and a rook. \n
 * - En Passant: a movimentation that captures a pawn with the en passant rule. \n
 * 
 * @see Move
*/
typedef enum {
    NORMAL,
    CAPTURE,
    CASTLE,
    EN_PASSANT
} MoveType;

struct Piece;

#endif
