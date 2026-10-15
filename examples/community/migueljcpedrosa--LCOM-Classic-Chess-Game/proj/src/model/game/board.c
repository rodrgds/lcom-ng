#include "board.h"
#include <stdlib.h>
#include "model/model_utils.h"

unsigned int square_size = SQUARE_SIZE;
unsigned int board_start = SCREEN_WIDTH / 2 - 4 * SQUARE_SIZE;

Board* (create_board)(Player* white_player, Player* black_player){

    Board* board = malloc(sizeof(Board));

    set_white_pieces(board, white_player);

    for (int i = 16; i < 48; i++){
        board->squares[i] = NULL;
    }

    set_black_pieces(board, black_player);

    return board;
}

void set_white_pieces(Board* board, Player* white_player){

    Color color = WHITE;

    board->squares[56] = create_piece(ROOK, color, create_position(56));
    board->squares[57] = create_piece(KNIGHT, color, create_position(57));
    board->squares[58] = create_piece(BISHOP, color, create_position(58));
    board->squares[59] = create_piece(QUEEN, color, create_position(59));
    board->squares[60] = create_piece(KING, color, create_position(60));
    board->squares[61] = create_piece(BISHOP, color, create_position(61));
    board->squares[62] = create_piece(KNIGHT, color, create_position(62));
    board->squares[63] = create_piece(ROOK, color, create_position(63));

    for (int i = 48; i < 56; i++){
        board->squares[i] = create_piece(PAWN, color, create_position(i));
    }

    for (int i = 0; i < 16; i++){
        white_player->pieces[i] = board->squares[i + 48];
    }
}

void set_black_pieces(Board* board, Player* black_player){

    Color color = BLACK;

    board->squares[0] = create_piece(ROOK, color, create_position(0));
    board->squares[1] = create_piece(KNIGHT, color, create_position(1));
    board->squares[2] = create_piece(BISHOP, color, create_position(2));
    board->squares[3] = create_piece(QUEEN, color, create_position(3));
    board->squares[4] = create_piece(KING, color, create_position(4));
    board->squares[5] = create_piece(BISHOP, color, create_position(5));
    board->squares[6] = create_piece(KNIGHT, color, create_position(6));
    board->squares[7] = create_piece(ROOK, color, create_position(7));

    for (int i = 0; i < 8; i++){
        board->squares[i + 8] = create_piece(PAWN, color, create_position(i + 8));
    }

    for (int i = 0; i < 16; i++){
        black_player->pieces[i] = board->squares[i];
    }
}

void destroy_board(Board* board){

    for (int i = 0; i < 64; i++){
        free(board->squares[i]);
    }

    free(board);
}
