#include <lcom/lcf.h>
#include <stdio.h>
#include <stdlib.h>
#include "game.h"

TetrisPiece create_piece(char type, int startX, int startY, int colorIndex) {
    TetrisPiece piece;
    piece.type = type;
    switch (type) {
        case 'T':
            piece.position[0][0] = startX;     piece.position[0][1] = startY;
            piece.position[1][0] = startX + 1; piece.position[1][1] = startY;
            piece.position[2][0] = startX + 2; piece.position[2][1] = startY;
            piece.position[3][0] = startX + 1; piece.position[3][1] = startY + 1;
            piece.pivot = 1; // Central block as pivot
            piece.colorIndex = 1;
            break;
        case 'I':
            piece.position[0][0] = startX; piece.position[0][1] = startY;
            piece.position[1][0] = startX; piece.position[1][1] = startY + 1;
            piece.position[2][0] = startX; piece.position[2][1] = startY + 2;
            piece.position[3][0] = startX; piece.position[3][1] = startY + 3;
            piece.pivot = 1;
            piece.colorIndex = 2;
            break;
        case 'L':
            piece.position[0][0] = startX;     piece.position[0][1] = startY;
            piece.position[1][0] = startX;     piece.position[1][1] = startY + 1;
            piece.position[2][0] = startX;     piece.position[2][1] = startY + 2;
            piece.position[3][0] = startX + 1; piece.position[3][1] = startY + 2;
            piece.pivot = 1; // Second block as pivot
            piece.colorIndex = 3;
            break;
        case 'Z':
            piece.position[0][0] = startX;     piece.position[0][1] = startY;
            piece.position[1][0] = startX + 1; piece.position[1][1] = startY;
            piece.position[2][0] = startX + 1; piece.position[2][1] = startY + 1;
            piece.position[3][0] = startX + 2; piece.position[3][1] = startY + 1;
            piece.pivot = 1; // Second block as pivot
            piece.colorIndex = 4;
            break;
        case 'S': // square
            piece.position[0][0] = startX;     piece.position[0][1] = startY;
            piece.position[1][0] = startX + 1; piece.position[1][1] = startY;
            piece.position[2][0] = startX;     piece.position[2][1] = startY + 1;
            piece.position[3][0] = startX + 1; piece.position[3][1] = startY + 1;
            piece.pivot = 0;
            piece.colorIndex = 5;
            break;
    }
    piece.isActive = true;
    return piece;
}

bool is_piece_at_bottom(TetrisPiece *piece, char screen[24][32], int colorScreen[24][32]) {
    for (int i = 0; i < 4; i++) {
        if (piece->position[i][1] == 23 || screen[piece->position[i][1] + 1][piece->position[i][0]] == 'B' || screen[piece->position[i][1] + 1][piece->position[i][0]] == 'P') {
            for (int i = 0; i < 4; i++) {
                colorScreen[piece->position[i][1]][piece->position[i][0]] = piece->colorIndex;
                screen[piece->position[i][1]][piece->position[i][0]] = 'P';
            }
            piece->isActive = false;
            return true;
        }
    }
    return false;
}

TetrisPiece generate_random_piece() {
    char types[] = {'T', 'I', 'L', 'Z', 'S'};
    char type = types[rand() % 5];
    int startX = 5 + rand() % 5;
    return create_piece(type, startX, 1, 1);
}

void move_piece(TetrisPiece *piece, int deltaX, int deltaY, char screen[24][32], int colorScreen[24][32]) {
    int oldPositions[4][2];
    for (int i = 0; i < 4; i++) {
        oldPositions[i][0] = piece->position[i][0];
        oldPositions[i][1] = piece->position[i][1];
        screen[piece->position[i][1]][piece->position[i][0]] = '-';
        colorScreen[piece->position[i][1]][piece->position[i][0]] = 0;
    }
    bool movePossible = true;
    for (int i = 0; i < 4; i++) {
        int newX = oldPositions[i][0] + deltaX;
        int newY = oldPositions[i][1] + deltaY;
        if (newX < 1 || newX > 14 || newY < 1 || newY >= 24 || screen[newY][newX] != '-') {
            movePossible = false;
            break;
        }
    }
    if (!movePossible) {
        for (int i = 0; i < 4; i++) {
            screen[oldPositions[i][1]][oldPositions[i][0]] = piece->type;
            colorScreen[oldPositions[i][1]][oldPositions[i][0]] = piece->colorIndex;
        }
        return;
    }
    for (int i = 0; i < 4; i++) {
        piece->position[i][0] = oldPositions[i][0] + deltaX;
        piece->position[i][1] = oldPositions[i][1] + deltaY;
        screen[piece->position[i][1]][piece->position[i][0]] = piece->type;
        colorScreen[piece->position[i][1]][piece->position[i][0]] = piece->colorIndex;
    }
    copy_piece_to_screen(piece, screen, colorScreen);
}

bool is_line_full(char screen[24][32], int line) {
    for (int i = 1; i < 15; i++) {
        if (screen[line][i] == '-') {
            return false;
        }
    }
    return true;
}

void check_and_clear_full_lines(char screen[24][32], int colorScreen[24][32], TetrisPiece pieces[], int num_pieces) {
    for (int i = 1; i < 23; i++) {
        if (is_line_full(screen, i)) {
            for (int j = 1; j < 15; j++) {
                screen[i][j] = '-';
                colorScreen[i][j] = 0;
            }
            for (int k = i; k > 1; k--) {
                for (int j = 1; j < 15; j++) {
                    screen[k][j] = screen[k - 1][j];
                    colorScreen[k][j] = colorScreen[k - 1][j];
                }
            }
            for (int p = 0; p < num_pieces; p++) {
                for (int b = 0; b < 4; b++) {
                    if (pieces[p].position[b][1] < i) {
                        pieces[p].position[b][1]++;
                    }
                }
            }
        }
    }
}

void draw(char screen[24][32], int colorScreen[24][32], TetrisPiece pieces[], int num_pieces) {
    clean_buffer();
    for (int p = 0; p < num_pieces; p++) {
        if (pieces[p].isActive) {
            for (int b = 0; b < 4; b++) {
                int px = pieces[p].position[b][0];
                int py = pieces[p].position[b][1];
                screen[py][px] = pieces[p].type;
                colorScreen[py][px] = pieces[p].colorIndex;
            }
        }
    }
    for (int i = 0; i < 24; i++) {
        for (int j = 0; j < 32; j++) {
            int colorIndex = colorScreen[i][j];
            if (j < 15 && colorIndex > 0 && colorIndex < 6) {
                draw_rectangle(j * 32, i * 32, 32, 32, colorIndex);
            }
            if (screen[i][j] == 'B') {
                draw_rectangle(j * 32, i * 32, 32, 32, 7);
            }
        }
    }
}

void rotate_piece(TetrisPiece *piece, char screen[24][32], int colorScreen[24][32]) {
    if (piece->type == 'S') {
        return;
    }
    int oldPositions[4][2];
    for (int i = 0; i < 4; i++) {
        oldPositions[i][0] = piece->position[i][0];
        oldPositions[i][1] = piece->position[i][1];
        screen[piece->position[i][1]][piece->position[i][0]] = '-';
        colorScreen[piece->position[i][1]][piece->position[i][0]] = 0;
    }
    int pivotX = piece->position[piece->pivot][0];
    int pivotY = piece->position[piece->pivot][1];
    for (int i = 0; i < 4; i++) {
        int newX = pivotX + pivotY - oldPositions[i][1];
        int newY = pivotY - pivotX + oldPositions[i][0];
        if (newX < 1 || newX > 14 || newY < 1 || newY >= 24 || screen[newY][newX] != '-') {
            for (int j = 0; j < 4; j++) {
                screen[oldPositions[j][1]][oldPositions[j][0]] = piece->type;
                colorScreen[oldPositions[j][1]][oldPositions[j][0]] = piece->colorIndex;
            }
            return;
        }
    }
    for (int i = 0; i < 4; i++) {
        piece->position[i][0] = pivotX + pivotY - oldPositions[i][1];
        piece->position[i][1] = pivotY - pivotX + oldPositions[i][0];
        screen[piece->position[i][1]][piece->position[i][0]] = piece->type;
        colorScreen[piece->position[i][1]][piece->position[i][0]] = piece->colorIndex;
    }
}

bool game_over(char screen[24][32]) {
    for (int i = 1; i < 15; i++) {
        if (screen[1][i] == 'P') {
            return true;
        }
    }
    return false;
}

int draw_centered_rectangles(int max_x, int max_y) {
    int rect_width = 300;
    int rect_height = 100;
    int gap = 30; 

    int top_rect_x = (max_x / 2) - (rect_width / 2);
    int top_rect_y = (max_y / 2) - rect_height - (gap / 2);

    int bottom_rect_x = (max_x / 2) - (rect_width / 2);
    int bottom_rect_y = (max_y / 2) + (gap / 2);

    if (draw_hline(top_rect_x, top_rect_y, rect_width, 1) != 0) return 1;
    if (draw_vline(top_rect_x, top_rect_y, rect_height, 1) != 0) return 1;
    if (draw_vline(top_rect_x + rect_width, top_rect_y, rect_height, 1) != 0) return 1;
    if (draw_hline(top_rect_x, top_rect_y + rect_height, rect_width, 1) != 0) return 1;

    if (draw_hline(bottom_rect_x, bottom_rect_y, rect_width, 1) != 0) return 1;
    if (draw_vline(bottom_rect_x, bottom_rect_y, rect_height, 1) != 0) return 1;
    if (draw_vline(bottom_rect_x + rect_width, bottom_rect_y, rect_height, 1) != 0) return 1;
    if (draw_hline(bottom_rect_x, bottom_rect_y + rect_height, rect_width, 1) != 0) return 1;

    return 0; 
}

void draw_word_in_rectangle(int rect_x, int rect_y, const xpm_row_t* word[], int word_length) {
    int letter_width = 7;  
    int letter_height = 9;
    int spacing = 2;

    int total_width = (letter_width + spacing) * word_length - spacing;
    int start_x = rect_x + (300 - total_width) / 2;
    int start_y = rect_y + (100 - letter_height) / 2;

    for (int i = 0; i < word_length; i++) {
        if (draw_xpm(start_x + i * (letter_width + spacing), start_y, word[i]) != 0) {
            printf("Failed to draw letter %d of the word.\n", i);
        }
    }
}

void drop_until_bottom(TetrisPiece *piece, char screen[24][32], int colorScreen[24][32]) {
    while (!is_piece_at_bottom(piece, screen, colorScreen)) {
        move_piece(piece, 0, 1, screen, colorScreen);
    }
}

void handle_game_over(GameState *state, char screen[24][32], int colorScreen[24][32], TetrisPiece pieces[], int *countpiece) {
    *state = GAME_OVER;
    for (int i = 0; i < 24; i++) {
        for (int j = 0; j < 32; j++) {
            screen[i][j] = '-';
            colorScreen[i][j] = 63;
        }
    }
    draw(screen, colorScreen, pieces, *countpiece);
    swap_buffer();
}

void copy_piece_to_screen(TetrisPiece *piece, char screen[24][32], int colorScreen[24][32]) {
    for (int i = 0; i < 4; i++) {
        colorScreen[piece->position[i][1]][piece->position[i][0]] = piece->colorIndex;
        screen[piece->position[i][1]][piece->position[i][0]] = piece->type;
    }
}


void reset_screen(char screen[24][32], int colorScreen[24][32]) {
    for (int i = 0; i < 24; i++) {
        for (int j = 0; j < 32; j++) {
            if (j == 0 || j >= 14 || i == 0 || i == 23) { 
                screen[i][j] = 'B';  
                colorScreen[i][j] = 7; 
            } else if (j < 15) {
                screen[i][j] = '-';
                colorScreen[i][j] = 0;
            } else {
                screen[i][j] = ' ';  
                colorScreen[i][j] = 0;
            }
        }
    }
}


bool is_within_rectangle(int x, int y, int rect_x, int rect_y, int rect_width, int rect_height) {
    return (x >= rect_x && x <= rect_x + rect_width && y >= rect_y && y <= rect_y + rect_height);
}
