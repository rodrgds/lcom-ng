#include "chess.h"

void load_board() {
    board[0][0] = (Piece) {ROOK, BLACK};
    board[0][1] = (Piece) {KNIGHT, BLACK};
    board[0][2] = (Piece) {BISHOP, BLACK};
    board[0][3] = (Piece) {QUEEN, BLACK};
    board[0][4] = (Piece) {KING, BLACK};
    board[0][5] = (Piece) {BISHOP, BLACK};
    board[0][6] = (Piece) {KNIGHT, BLACK};
    board[0][7] = (Piece) {ROOK, BLACK};
    for (int i = 0; i < 8; i++) {
        board[1][i] = (Piece) {PAWN, BLACK};
    }

    for (int i = 2; i < 6; i++) {
        for (int j = 0; j < 8; j++) {
            board[i][j] = (Piece) {EMPTY, UNDEFINED};
        }
    }

    for (int i = 0; i < 8; i++) {
        board[6][i] = (Piece) {PAWN, WHITE};
    }
    board[7][0] = (Piece) {ROOK, WHITE};
    board[7][1] = (Piece) {KNIGHT, WHITE};
    board[7][2] = (Piece) {BISHOP, WHITE};
    board[7][3] = (Piece) {QUEEN, WHITE};
    board[7][4] = (Piece) {KING, WHITE};
    board[7][5] = (Piece) {BISHOP, WHITE};
    board[7][6] = (Piece) {KNIGHT, WHITE};
    board[7][7] = (Piece) {ROOK, WHITE};

    copy_board(prev, board);

    deselect_piece(); 
    game_over = false; 
    white_turn = true;

    clock_init();
}

//TODO - return sometimes crashed the game

int draw_board() {
    int dark_board_condition = (time_rtc[0] > 20 || time_rtc[0] < 8);
    if (draw_sprite(dark_board_condition ? board_dark_img : board_img, 0, 0)) return 1;    

    return 0;
}

int draw_piece(int i, int j, uint32_t color) {
    Piece piece = board[i][j];

    switch (piece.type) {
        case KING:
            return draw_king(j * SQUARE_SIZE, i * SQUARE_SIZE, color);
        case QUEEN:
            return draw_queen(j * SQUARE_SIZE, i * SQUARE_SIZE, color);
        case BISHOP:
            return draw_bishop(j * SQUARE_SIZE, i * SQUARE_SIZE, color);
        case KNIGHT:
            return draw_knight(j * SQUARE_SIZE, i * SQUARE_SIZE, color);
        case ROOK:
            return draw_rook(j * SQUARE_SIZE, i * SQUARE_SIZE, color);
        case PAWN:  
            return draw_pawn(j * SQUARE_SIZE, i * SQUARE_SIZE, color);

        default:
            return 0;
    }
}

int draw_pieces() {
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            Piece piece = board[i][j];
            
            if (piece.type == EMPTY) continue;

            if (draw_piece(i, j, piece.color)) return 1;
        }
    }

    return 0;
}

void select_piece(int row, int col) {
    if (game_over) return;
    
    Piece piece = board[row][col];

    if (piece.type == EMPTY) return;

    if (piece.color == WHITE && !white_turn) return;

    if (piece.color == BLACK && white_turn) return;

    sel_row = row;
    sel_col = col;

    selected = true;

    return;
}

void mouse_select_piece(int x, int y) {
    int row = y / SQUARE_SIZE;
    int col = x / SQUARE_SIZE;

    if (row < 0 || row > 7 || col < 0 || col > 7) return;

    select_piece(row, col);

    return;
}

void deselect_piece() {
    sel_row = -1;
    sel_col = -1;

    selected = false;

    return;
}

bool is_selected() {
    return selected;
}

Position get_selected() {
    return (Position) {sel_row, sel_col};
}

int draw_selected() {
    if (!selected) return 0;

    if (draw_sprite(sel_img, sel_col * SQUARE_SIZE, sel_row * SQUARE_SIZE)) return 1;

    return 0;
}

void move_piece(int row, int col) {
    if (game_over) return;
    
    if (!selected) return;

    if (row == sel_row && col == sel_col) return;

    if (is_valid_move(row, col)) {
        copy_board(prev, board);
        board[row][col] = board[sel_row][sel_col];
        board[sel_row][sel_col] = (Piece) {EMPTY, UNDEFINED};

        Piece piece = board[row][col];
        if (piece.type == PAWN) {
            if (row == 0 || row == 7) {
                board[row][col] = (Piece) {QUEEN, piece.color};
            }
        }

        white_turn = !white_turn;
        clock_set(white_turn);   
    }

    deselect_piece();

    select_piece(row, col);

    return;
}

void mouse_move_piece(int xf, int yf) {
    int row = yf / SQUARE_SIZE;
    int col = xf / SQUARE_SIZE;

    if (row < 0 || row > 7 || col < 0 || col > 7) return;

    move_piece(row, col);

    return;
}

void keyboard_handle_input(uint8_t scancode) {
    switch (scancode) {
        case KEY_BACKSPACE:
            deselect_piece();
            kbd_row = -1; 
            kbd_col = -1;
            break;
        
        case KEY_1: 
            kbd_row = 7; break;
        case KEY_2: 
            kbd_row = 6; break;
        case KEY_3: 
            kbd_row = 5; break;
        case KEY_4: 
            kbd_row = 4; break;
        case KEY_5: 
            kbd_row = 3; break;
        case KEY_6: 
            kbd_row = 2; break;
        case KEY_7: 
            kbd_row = 1; break;
        case KEY_8: 
            kbd_row = 0; break;
        case KEY_A: 
            kbd_col = 0; break;
        case KEY_B: 
            kbd_col = 1; break;
        case KEY_C: 
            kbd_col = 2; break;
        case KEY_D: 
            kbd_col = 3; break;
        case KEY_E: 
            kbd_col = 4; break;
        case KEY_F: 
            kbd_col = 5; break;
        case KEY_G: 
            kbd_col = 6; break;
        case KEY_H: 
            kbd_col = 7; break;

        default:
            break;        
    }

    if (!selected && kbd_row != -1 && kbd_col != -1) {
        select_piece(kbd_row, kbd_col);
        kbd_row = -1;
        kbd_col = -1;
        return;
    }

    if (kbd_row != -1 && kbd_col != -1) {
        move_piece(kbd_row, kbd_col);
        kbd_row = -1;
        kbd_col = -1;
    }

    return;
}

bool is_valid_move(int row, int col) {
    int size;
    Position *valid_moves = get_valid_moves(&size);

    for (int i = 0; i < size; i++) {
        if (valid_moves[i].row == row && valid_moves[i].col == col && can_move(row, col)) {
            free(valid_moves);
            return true;
        }
    }

    free(valid_moves);
    return false;
}

Position *get_valid_moves(int *size) {
    Piece sel_piece = board[sel_row][sel_col];

    switch (sel_piece.type) {
        case KING:
            return get_valid_king_moves(size);
        case QUEEN:
            return get_valid_queen_moves(size);
        case BISHOP:
            return get_valid_bishop_moves(size);
        case KNIGHT:
            return get_valid_knight_moves(size);
        case ROOK:
            return get_valid_rook_moves(size);
        case PAWN:
            return get_valid_pawn_moves(size);

        default:
            return NULL;
    }
}

Position *get_moves(int *size, int row, int col) {
    int temp_row = sel_row;
    int temp_col = sel_col;

    sel_row = row;
    sel_col = col;

    Position *valid_moves = get_valid_moves(size);

    sel_row = temp_row;
    sel_col = temp_col;

    return valid_moves;
}

Position get_king_position() {
    for (int row = 0; row < 8; row++) {
        for (int col = 0; col < 8; col++) {
            Piece piece = board[row][col];

            if (piece.type == KING && piece.color == (white_turn ? WHITE : BLACK)) {
                return (Position) {row, col};
            }
        }
    }

    return (Position) {-1, -1};
}

bool is_check() {
    Position king_pos = get_king_position();

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            Piece piece = board[i][j];

            if (piece.type == EMPTY) continue;

            if (piece.color == (white_turn ? BLACK : WHITE)) {
                int size;
                Position *valid_moves = get_moves(&size, i, j);

                for (int k = 0; k < size; k++) {
                    if (valid_moves[k].row == king_pos.row && valid_moves[k].col == king_pos.col) {
                        free(valid_moves);
                        return true;
                    }
                }

                free(valid_moves);
            }
        }
    }

    return false;
}

bool can_move(int row, int col) {
    Piece temp[8][8];
    copy_board(temp, board);

    board[row][col] = board[sel_row][sel_col];
    board[sel_row][sel_col] = (Piece) {EMPTY, UNDEFINED};    

    bool check = is_check();

    copy_board(board, temp);
    return !check;
}

bool is_checkmate() {
    if (!is_check()) return false;
    
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {            
            Piece piece = board[i][j];

            if (piece.type == EMPTY) continue;

            if (piece.color == (white_turn ? WHITE : BLACK)) {
                int size;
                Position *valid_moves = get_moves(&size, i, j);

                int temp_row = sel_row;
                int temp_col = sel_col;

                sel_row = i;
                sel_col = j;

                for (int k = 0; k < size; k++) {
                    if (can_move(valid_moves[k].row, valid_moves[k].col)) {

                        sel_row = temp_row;
                        sel_col = temp_col;

                        free(valid_moves);
                        return false;
                    }
                }

                sel_row = temp_row;
                sel_col = temp_col;

                free(valid_moves);
            }
        }
    }

    return true;
}

void copy_board(Piece dest[8][8], Piece src[8][8]) {
    for (int i = 0; i < 8; i++) {
        memcpy(&dest[i], src[i], 8 * sizeof(Piece));
    }

    return;
}

void undo_move() {
    if (game_over) return;  
    
    if (memcmp(board, prev, 8 * 8 * sizeof(Piece)) == 0) return;
    
    copy_board(board, prev);
    white_turn = !white_turn;
    selected = false;
    clock_set(white_turn);

    return;
}

void set_game_over() {
    game_over = true;
    selected = false;
    clock_stop();
}


/* Piece movement functions */

Position *get_valid_king_moves(int *size) {
    Position *moves = malloc(8 * sizeof(Position));

    int rows[] = {-1, -1, -1, 0, 0, 1, 1, 1};
    int cols[] = {-1, 0, 1, -1, 1, -1, 0, 1};

    int count = 0;

    for (int i = 0; i < 8; i++) {
        int row = sel_row + rows[i];
        int col = sel_col + cols[i];

        if (row < 0 || row > 7 || col < 0 || col > 7) continue;

        if (board[row][col].type == EMPTY || board[row][col].color != board[sel_row][sel_col].color) {
            moves[count++] = (Position) {row, col};
        }
    }

    *size = count;

    return moves;
}

Position *get_valid_queen_moves(int *size) {
    Position *moves = malloc(27 * sizeof(Position));

    int rows[] = {-1, -1, -1, 0, 0, 1, 1, 1};
    int cols[] = {-1, 0, 1, -1, 1, -1, 0, 1};

    int count = 0;

    for (int i = 0; i < 8; i++) {
        int row = sel_row + rows[i];
        int col = sel_col + cols[i];

        while (row >= 0 && row <= 7 && col >= 0 && col <= 7) {
            if (board[row][col].type == EMPTY) {
                moves[count++] = (Position) {row, col};
            } else if (board[row][col].color != board[sel_row][sel_col].color) {
                moves[count++] = (Position) {row, col};
                break;
            } else {
                break;
            }

            row += rows[i];
            col += cols[i];
        }
    }

    *size = count;

    return moves;
}

Position *get_valid_bishop_moves(int *size) {
    Position *moves = malloc(13 * sizeof(Position));

    int rows[] = {-1, -1, 1, 1};
    int cols[] = {-1, 1, -1, 1};

    int count = 0;

    for (int i = 0; i < 4; i++) {
        int row = sel_row + rows[i];
        int col = sel_col + cols[i];

        while (row >= 0 && row <= 7 && col >= 0 && col <= 7) {
            if (board[row][col].type == EMPTY) {
                moves[count++] = (Position) {row, col};
            } else if (board[row][col].color != board[sel_row][sel_col].color) {
                moves[count++] = (Position) {row, col};
                break;
            } else {
                break;
            }

            row += rows[i];
            col += cols[i];
        }
    }

    *size = count;

    return moves;
}


Position *get_valid_knight_moves(int *size) {
    Position *moves = malloc(8 * sizeof(Position));

    int rows[] = {-2, -2, -1, -1, 1, 1, 2, 2};
    int cols[] = {-1, 1, -2, 2, -2, 2, -1, 1};

    int count = 0;

    for (int i = 0; i < 8; i++) {
        int row = sel_row + rows[i];
        int col = sel_col + cols[i];

        if (row < 0 || row > 7 || col < 0 || col > 7) continue;

        if (board[row][col].type == EMPTY || board[row][col].color != board[sel_row][sel_col].color) {
            moves[count++] = (Position) {row, col};
        }
    }

    *size = count;

    return moves;
}

Position *get_valid_rook_moves(int *size) {
    Position *moves = malloc(14 * sizeof(Position));

    int rows[] = {-1, -1, 0, 0, 1, 1};
    int cols[] = {0, 1, -1, 1, 0, 1};

    int count = 0;

    for (int i = 0; i < 6; i++) {
        int row = sel_row + rows[i];
        int col = sel_col + cols[i];

        while (row >= 0 && row <= 7 && col >= 0 && col <= 7) {
            if (board[row][col].type == EMPTY) {
                moves[count++] = (Position) {row, col};
            } else if (board[row][col].color != board[sel_row][sel_col].color) {
                moves[count++] = (Position) {row, col};
                break;
            } else {
                break;
            }

            row += rows[i];
            col += cols[i];
        }
    }

    *size = count;

    return moves;
}

Position *get_valid_pawn_moves(int *size) {
    Position *moves = malloc(4 * sizeof(Position));

    int count = 0;

    if (sel_row == 1 && board[sel_row][sel_col].color == BLACK) {
        if (board[sel_row + 1][sel_col].type == EMPTY) {
            moves[count++] = (Position) {sel_row + 1, sel_col};

            if (board[sel_row + 2][sel_col].type == EMPTY) {
                moves[count++] = (Position) {sel_row + 2, sel_col};
            }
        }
    }

    if (sel_row == 6 && board[sel_row][sel_col].color == WHITE) {
        if (board[sel_row - 1][sel_col].type == EMPTY) {
            moves[count++] = (Position) {sel_row - 1, sel_col};

            if (board[sel_row - 2][sel_col].type == EMPTY) {
                moves[count++] = (Position) {sel_row - 2, sel_col};
            }
        }
    }

    if (board[sel_row][sel_col].color == BLACK) {
        if (board[sel_row + 1][sel_col].type == EMPTY) {
            moves[count++] = (Position) {sel_row + 1, sel_col};
        }

        if (board[sel_row + 1][sel_col - 1].type != EMPTY && board[sel_row + 1][sel_col - 1].color != board[sel_row][sel_col].color) {
            moves[count++] = (Position) {sel_row + 1, sel_col - 1};
        }

        if (board[sel_row + 1][sel_col + 1].type != EMPTY && board[sel_row + 1][sel_col + 1].color != board[sel_row][sel_col].color) {
            moves[count++] = (Position) {sel_row + 1, sel_col + 1};
        }
    }

    if (board[sel_row][sel_col].color == WHITE && sel_col - 1 >= 0 && sel_col + 1 <= 7) {
        if (board[sel_row - 1][sel_col].type == EMPTY) {
            moves[count++] = (Position) {sel_row - 1, sel_col};
        }

        if (board[sel_row - 1][sel_col - 1].type != EMPTY && board[sel_row - 1][sel_col - 1].color != board[sel_row][sel_col].color) {
            moves[count++] = (Position) {sel_row - 1, sel_col - 1};
        }

        if (board[sel_row - 1][sel_col + 1].type != EMPTY && board[sel_row - 1][sel_col + 1].color != board[sel_row][sel_col].color) {
            moves[count++] = (Position) {sel_row - 1, sel_col + 1};
        }
    }

    *size = count;

    return moves;
}


/* Auxiliary drawing functions */

int draw_king(int x, int y, uint32_t color) {
    if (color == BLACK) {
        if (draw_sprite(bK, x, y)) return 1;
    } else {
        if (draw_sprite(wK, x, y)) return 1;
    }

    return 0;
}

int draw_queen(int x, int y, uint32_t color) {
    if (color == BLACK) {
        if (draw_sprite(bQ, x, y)) return 1;
    } else {
        if (draw_sprite(wQ, x, y)) return 1;
    }

    return 0;
}

int draw_bishop(int x, int y, uint32_t color) {
    if (color == BLACK) {
        if (draw_sprite(bB, x, y)) return 1;
    } else {
        if (draw_sprite(wB, x, y)) return 1;
    }

    return 0;
}

int draw_knight(int x, int y, uint32_t color) {
    if (color == BLACK) {
        if (draw_sprite(bN, x, y)) return 1;
    } else {
        if (draw_sprite(wN, x, y)) return 1;
    }

    return 0;
}

int draw_rook(int x, int y, uint32_t color) {
    if (color == BLACK) {
        if (draw_sprite(bR, x, y)) return 1;
    } else {
        if (draw_sprite(wR, x, y)) return 1;
    }

    return 0;
}

int draw_pawn(int x, int y, uint32_t color) {
    if (color == BLACK) {
        if (draw_sprite(bP, x, y)) return 1;
    } else {
        if (draw_sprite(wP, x, y)) return 1;
    }

    return 0;
}
