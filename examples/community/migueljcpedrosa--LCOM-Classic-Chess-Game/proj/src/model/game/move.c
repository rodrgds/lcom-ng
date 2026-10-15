#include "move.h"
#include "pieces.h"

Move create_normal_move(Position origin, Position destination, struct Piece* piece, struct Piece* secondary_piece){

    Move move;
    move.origin = origin;
    move.destination = destination;
    move.piece = piece;
    move.old_type = piece->type;
    move.had_moved = piece->has_moved;
    move.secondary_piece = secondary_piece;
    move.type = NORMAL;

    return move;
}

Move create_capture(Position origin, Position destination, struct Piece* piece, struct Piece* secondary_piece){

    Move move;
    move.origin = origin;
    move.destination = destination;
    move.piece = piece;
    move.old_type = piece->type;
    move.had_moved = piece->has_moved;
    move.secondary_piece = secondary_piece;
    move.type = CAPTURE;

    return move;
}

Move create_castle(Position origin, Position destination, struct Piece* piece, struct Piece* secondary_piece){

    Move move;
    move.origin = origin;
    move.destination = destination;
    move.piece = piece;
    move.old_type = piece->type;
    move.had_moved = piece->has_moved;
    move.secondary_piece = secondary_piece;
    move.type = CASTLE;

    return move;
}

Move create_en_passant(Position origin, Position destination, struct Piece* piece, struct Piece* secondary_piece){

    Move move;
    move.origin = origin;
    move.destination = destination;
    move.piece = piece;
    move.old_type = piece->type;
    move.had_moved = piece->has_moved;
    move.secondary_piece = secondary_piece;
    move.type = EN_PASSANT;

    return move;
}
