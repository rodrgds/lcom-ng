/**
 * @file game_controller.h
 * @brief This file contains the definition of the game controller module.
 * 
 * This handles all inputs of the game.
 */

#ifndef _GAME_CTRL_H_
#define _GAME_CTRL_H_

#include "../model/cursor/cursor_input.h"
#include "../model/game/game.h"

//! @brief Represents the currently selected piece.
extern Piece* selected_piece;

/**
 * @brief Checks if the game is over.
 * 
 * This function checks after every move if a stalemate or checkmate has been reached.
 * 
 * @return Returns true if the game is over, false otherwise.
 */
bool (check_game_over)();

/**
 * @brief Setup the bot waiting timer for it's move.
 * 
 * This function sets up the bot timer, so that it waits a random amount of time.
 * Lower times have a higher chance of being generated.
 * 
 * @param player the bot player.
 */
void (setup_bot_timer)(Player* player);

/**
 * @brief Gets all the possible moves for the bot.
 * 
 * @param bot the bot player.
 * @param possibleMoves the array where the possible moves will be stored.
 * @param num_moves the number of possible moves.
 */
void (get_all_moves)(Player* bot, Move* possibleMoves, int* num_moves);

/**
 * @brief Handles move execution for the bot.
 * 
 * It chooses a random move from the possible moves and executes it, after
 * a random amount of time.
 * 
 * @param bot the bot player.
 * @param possibleMoves the array where the possible moves will be stored.
 * @param num_moves the number of possible moves.
 * 
 * @return Returns the best move for the bot.
 */
void (handle_bot_play)(Player* bot);

/**
 * @brief Handles the cursor input.
 * 
 * This function handles the cursor input, by selecting a piece or moving it.
 * 
 * @param input the cursor input.
 */
void (game_cursor)(CursorInput* input);

/**
 * @brief Handles the timer input.
 * 
 * This function updates the players timers and handles the bot move.
 */
void (game_timer)();

/**
 * @brief Handles the keyboard input.
 * 
 * This function handles the keyboard input. 
 * It may be used to quit the game.
 * 
 * @param scan_code the scan code of the key pressed.
 * @param size the size of the scan code.
 */
void (game_kbd)(uint8_t scan_code[2], int size);

#endif
