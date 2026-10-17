#include "Board.h"


// Initializes the board with the given width and height
void board_init(Board* board, int width, int height) {
    board->width = width;
    board->height = height;

    board->cards = malloc(height * sizeof(Card*));
    for (int i = 0; i < height; ++i) {
        board->cards[i] = malloc(width * sizeof(Card));
    }
}

// Allocate memory for the 2D array of cards
void board_draw(const Board* board, const Player* player) {
    sprite_desenhar(background, 0, 0);    // Draw background
    player_draw(player);                   // Draw player
    // Draw all cards on the board
    for (int i = 0; i < board->height; i++) {
        for (int j = 0; j < board->width; j++) {
            card_draw(&board->cards[i][j]);
        }
    }
}
