#ifndef BOARD_H
#define BOARD_H


#include "../card/Card.h" 
#include "../player/Player.h" 

/**
 * @brief Structure representing the game board.
 */
typedef struct {
    int width;    /**< Width of the board (number of columns). */
    int height;   /**< Height of the board (number of rows). */
    Card** cards;  /**< 2D array of cards on the board. */
} Board;

/**
 * @brief Initializes the game board with the specified dimensions.
 * 
 * Allocates memory for a 2D array of cards based on the provided width and height.
 * 
 * @param board Pointer to the Board structure to initialize.
 * @param width Number of columns in the board.
 * @param height Number of rows in the board.
 */
void board_init(Board* board, int width, int height);

/**
 * @brief Draws the board and the player on the screen.
 * 
 * Renders the background, the player, and all cards on the board.
 * 
 * @param board Pointer to the Board structure to draw.
 * @param player Pointer to the Player structure to draw.
 */
void board_draw(const Board* board, const Player* player); 

#endif
