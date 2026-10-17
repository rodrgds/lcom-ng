/**
 * This module contains all functions that handle the logic of the game (collisions mostly)
 * Jorge Vale: 40%
 * Telmo Barros: 60%
 * Relative Weight: 5%
 */

#ifndef __LOGIC_H
#define __LOGIC_H

#include "character.h"
#include "piece.h"
#include "game.h"

/** @defgroup Logic Logic
 * @{
 * Functions to verify collisions
 */

/**
 * @brief Verifies if a piece is able to move left or right in the board
 *
 * @param piece Actual_piece normally, to verify if it can change its position in x axys
 * @param board Board that contains borders and previous played pieces
 * @param dir Direction to check (0 to left, 1 to right)
 *
 * @return Return 0 upon sucess and 1 upon failure
 */
int can_piece_move_x(Piece * piece, Board * board, unsigned int dir);

/**
 * @brief Verifies if a piece is able to fall in a board (move down)
 *
 * @param piece Actual_piece normally, to verify if it can change is position downwards
 * @param board Board that contains borders and previous played pieces
 *
 * @return Return 0 upon sucess and 1 upon failure
 */
int can_piece_fall(Piece * piece, Board * board);

/**
 * @brief Verifies if a piece is able to be placed in the board
 *
 * @param piece Actual_piece normally after rotation, to verify if it can be placed without collide with other pieces or borders
 * @param board Board that contains borders and previous played pieces
 *
 * @return Return 0 upon sucess and 1 upon failure
 */
int can_piece_be_placed(Piece * piece, Board * board);

/**
 * @brief Verifies if the character is able to move left or right
 *
 * @param character Character to verify if it can change its position in x axys
 * @param dir Direction to check (0 to left, 1 to right)
 *
 * @return Return 0 upon sucess and 1 upon failure
 */
int can_char_move_x(Character * character, unsigned int dir);

/**
 * @brief Verifies if a character is able to fall (move down), if there is empty space under it
 *
 * @param character Character to verify if it can change is position downwards
 *
 * @return Return 0 upon sucess and 1 upon failure
 */
int can_char_fall(Character * character);

/**
 * @brief Verifies if a character is able to jump (move up), if there is empty space above it
 *
 * @param character Character to verify if it can change is position upwards
 *
 * @return Return 0 upon sucess and 1 upon failure
 */
int can_char_jump(Character * character);

int char_piece_collision(Character * character,Piece * piece);

#endif /* __LOGIC_H */
