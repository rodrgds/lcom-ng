/**
 * @file game
 * @author Diogo Pintado, Gabriel Machado Jr., José Costa, Linda Rodrigues
 * 
 *
 * @brief Contains functions and structures related to the game logic and functionality.
 * 
 */
#ifndef _GAME_H_
#define _GAME_H_

#include "game_state.h"
#include "board.h"
#include "piece_move.h"


/**
 * @brief Plays the game with the given piece.
 * 
 * @param piece The piece to play with.
 */
void play_game(Piece * piece);

/**
 * @brief Handles the movement input for the current piece.
 * 
 * @param scancode The scancode to handle.
 * @param piece The current piece.
 */
void movement_input(uint8_t * scancode,Piece * piece);

/**
 * @brief Checks if the board has last line full.
 * 
 * @param piece The board to check.
 * @return true if last line full, false otherwise.
 * 
 */
bool check_full_line(Piece *piece);

/**
 * @brief Moves the board down one row.
 * 
 * @param board The board to move.
 */
void move_down_board(Board * board);

/**
 * @brief Loads the XPM image.
 * 
 */
void load_score_xpm();

/**
 * @brief Draws the score XPM image.
 * 
 */
void draw_score();

/**
 * @brief Checks for game over in game
 * 
 * @param piece The piece which is being played
*/
bool check_gameover(Piece * piece);

void scoreboard_files_init();

void write_score();

void load_game_over();

void write_game_over();

#endif // _GAME_H_
