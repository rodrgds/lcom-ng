/**
 * @file game.h
 * @brief Functions and data structures for the Tetris game.
 */

#ifndef GAME_H
#define GAME_H

#include <lcom/lcf.h>
#include <stdbool.h>
#include "video.h"
#include "stdint.h"

/**
 * @brief Game states for the Tetris game.
 */
typedef enum {
    MAIN_MENU,
    GAME,
    GAME_OVER   
} GameState;

/**
 * @brief Structure representing a Tetris piece.
 */
typedef struct {
    char type;            /**< The type of the Tetris piece. */
    int position[4][2];   /**< The positions of the blocks forming the piece. */
    int pivot;            /**< The pivot index for rotation. */
    int colorIndex;       /**< The color index of the piece. */
    bool isActive;        /**< Whether the piece is active. */
} TetrisPiece;

/**
 * @brief Creates a Tetris piece of the given type at the specified starting position.
 * 
 * @param type The type of the Tetris piece.
 * @param startX The starting X position.
 * @param startY The starting Y position.
 * @param colorIndex The color index of the piece.
 * @return The created Tetris piece.
 */
TetrisPiece create_piece(char type, int startX, int startY, int colorIndex);

/**
 * @brief Checks if the Tetris piece is at the bottom.
 * 
 * @param piece The Tetris piece to check.
 * @param screen The game screen.
 * @param colorScreen The color screen.
 * @return True if the piece is at the bottom, false otherwise.
 */
bool is_piece_at_bottom(TetrisPiece *piece, char screen[24][32], int colorScreen[24][32]);

/**
 * @brief Generates a random Tetris piece.
 * 
 * @return The generated Tetris piece.
 */
TetrisPiece generate_random_piece();

/**
 * @brief Moves the Tetris piece by the specified deltas.
 * 
 * @param piece The Tetris piece to move.
 * @param deltaX The delta in the X direction.
 * @param deltaY The delta in the Y direction.
 * @param screen The game screen.
 * @param colorScreen The color screen.
 */
void move_piece(TetrisPiece *piece, int deltaX, int deltaY, char screen[24][32], int colorScreen[24][32]);

/**
 * @brief Checks if a line on the screen is full.
 * 
 * @param screen The game screen.
 * @param line The line to check.
 * @return True if the line is full, false otherwise.
 */
bool is_line_full(char screen[24][32], int line);

/**
 * @brief Checks and clears full lines on the screen.
 * 
 * @param screen The game screen.
 * @param colorScreen The color screen.
 * @param pieces The array of Tetris pieces.
 * @param num_pieces The number of pieces.
 */
void check_and_clear_full_lines(char screen[24][32], int colorScreen[24][32], TetrisPiece pieces[], int num_pieces);

/**
 * @brief Draws the game screen.
 * 
 * @param screen The game screen.
 * @param colorScreen The color screen.
 * @param pieces The array of Tetris pieces.
 * @param num_pieces The number of pieces.
 */
void draw(char screen[24][32], int colorScreen[24][32], TetrisPiece pieces[], int num_pieces);

/**
 * @brief Rotates a Tetris piece.
 * 
 * @param piece The Tetris piece to rotate.
 * @param screen The game screen.
 * @param colorScreen The color screen.
 */
void rotate_piece(TetrisPiece *piece, char screen[24][32], int colorScreen[24][32]);

/**
 * @brief Checks if the game is over.
 * 
 * @param screen The game screen.
 * @return True if the game is over, false otherwise.
 */
bool game_over(char screen[24][32]);

/**
 * @brief Draws two centered rectangles on the screen.
 * 
 * @param max_x The maximum X dimension of the screen.
 * @param max_y The maximum Y dimension of the screen.
 * @return 0 upon success, 1 otherwise.
 */
int draw_centered_rectangles(int max_x, int max_y);

/**
 * @brief Draws a word inside a rectangle.
 * 
 * @param rect_x The X coordinate of the rectangle.
 * @param rect_y The Y coordinate of the rectangle.
 * @param word The word to draw.
 * @param word_length The length of the word.
 */
void draw_word_in_rectangle(int rect_x, int rect_y, const xpm_row_t* word[], int word_length);

/**
 * @brief Drops a Tetris piece until it reaches the bottom.
 * 
 * @param piece The Tetris piece to drop.
 * @param screen The game screen.
 * @param colorScreen The color screen.
 */
void drop_until_bottom(TetrisPiece *piece, char screen[24][32], int colorScreen[24][32]);

/**
 * @brief Handles the game over state.
 * 
 * @param state The current game state.
 * @param screen The game screen.
 * @param colorScreen The color screen.
 * @param pieces The array of Tetris pieces.
 * @param countpiece The count of pieces.
 */
void handle_game_over(GameState *state, char screen[24][32], int colorScreen[24][32], TetrisPiece pieces[], int *countpiece);

/**
 * @brief Copies the Tetris piece to the screen.
 * 
 * @param piece The Tetris piece to copy.
 * @param screen The game screen.
 * @param colorScreen The color screen.
 */
void copy_piece_to_screen(TetrisPiece *piece, char screen[24][32], int colorScreen[24][32]);

/**
 * @brief Resets the game screen and color screen to their initial state.
 * 
 * @param screen The game screen.
 * @param colorScreen The color screen.
 */
void reset_screen(char screen[24][32], int colorScreen[24][32]);

/**
 * @brief Checks if a point is within a rectangle.
 * 
 * @param x The X coordinate of the point.
 * @param y The Y coordinate of the point.
 * @param rect_x The X coordinate of the rectangle's top-left corner.
 * @param rect_y The Y coordinate of the rectangle's top-left corner.
 * @param rect_width The width of the rectangle.
 * @param rect_height The height of the rectangle.
 * @return True if the point is within the rectangle, false otherwise.
 */
bool is_within_rectangle(int x, int y, int rect_x, int rect_y, int rect_width, int rect_height);

#endif // GAME_H
