#ifndef DEVICESCONTROLLER_H
#define DEVICESCONTROLLER_H

#include <lcom/lcf.h>

#include <stdint.h>
#include <stdio.h>

#include "model/game.h"
#include "model/board.h"
#include "../model/structGame.h"

#include "../viewer/menuViewer.h"
#include "../viewer/waitingViewer.h"
#include "../viewer/winViewer.h"

#include "../xpm/mouse.h"
#include "../xpm/mouseD.h"

#include "state/state.h"

#include "macros.h"

/**
 * @brief Handles keyboard input for the game based on its current state.
 * This function processes keyboard input (scancode) and modifies the game state
 * accordingly. It performs different actions based on the current state of the game,
 * such as MENU, WAIT, SETUP, PLAYING, RULES, and EXIT.
 * @param game A pointer to the Game structure containing the current game state and data.
 * @param scancode The scancode of the key that was pressed.
 * @param time A pointer to an integer representing the current time or timer value.
 * @return Always returns 0.
*/
int mouseController(Game *game, const struct packet pp, int *xx, int *yy);

int keyController(Game *game, uint8_t scancode, int *time);

/**
 * @brief Manages the game state transitions based on the timer.
 * 
 * This function updates the game state and board based on the elapsed time.
 * 
 * @param game A pointer to the Game structure containing the current game state and data.
 * @param time A pointer to an integer representing the current timer value.
 * @return Always returns 0.
 */
int timerController(Game *game, int *time);



//Funções auxiliares



/**
 * @brief Handles the menu state interactions based on the keyboard input.
 * 
 * This function processes the scancode input to navigate the menu options and
 * update the game state based on the selected menu option.
 * 
 * @param game A pointer to the Game structure containing the current game state and data.
 * @param board A pointer to the Board structure containing the current board state.
 * @param scancode The scancode of the key that was pressed.
 * @param time A pointer to an integer representing the current timer value.
 */
void handleMenuState(Game *game, Board *board, uint8_t scancode, int *time);

/**
 * @brief Handles the wait state interactions based on the keyboard input.
 * 
 * This function processes the scancode input to transition from the WAIT state to the SETUP state.
 * 
 * @param game A pointer to the Game structure containing the current game state and data.
 * @param board A pointer to the Board structure containing the current board state.
 * @param scancode The scancode of the key that was pressed.
 * @param time A pointer to an integer representing the current timer value.
 */
void handleWaitState(Game *game, Board *board, uint8_t scancode, int *time);

//Funções da mmovimentação dos navios

/**
 * @brief Moves the selected ship on the board based on the keyboard input.
 * 
 * This function processes the scancode input to move the selected ship on the board
 * in the specified direction (up, down, left, or right) if the move is valid.
 * 
 * @param board A pointer to the Board structure containing the current board state.
 * @param isPlayer1 A boolean indicating if it is player 1's board.
 * @param scancode The scancode of the key that was pressed.
 */
void moveShip(Board *board, bool isPlayer1, int scancode);

/**
 * @brief Resets the game to its initial state.
 * 
 * This function resets the game timer, sets the game state to MENU, reinitializes both game boards,
 * sets the current player to player 1, and draws the menu.
 * 
 * @param game A pointer to the Game structure containing the current game state and data.
 * @param time A pointer to an integer representing the current timer value.
 */
void resetGame(Game *game, int *time);


/**
 * @brief Handles the movement of the selected ship on the board.
 * 
 * This function processes the scancode input to move the selected ship in the specified 
 * direction (up, down, left, or right) if the scancode corresponds to a valid movement key.
 * 
 * @param board A pointer to the Board structure containing the current board state.
 * @param isPlayer1 A boolean indicating if it is player 1's board.
 * @param scancode The scancode of the key that was pressed.
 */
void handleMovement(Board *board, bool isPlayer1, int scancode);

/**
 * @brief Handles the placement of the selected ship at the next empty position on the board.
 * 
 * This function finds the next empty position for the selected ship on the board, updates its 
 * coordinates accordingly, and draws the ship at the new position.
 * 
 * @param board A pointer to the Board structure containing the current board state.
 * @param isPlayer1 A boolean indicating if it is player 1's board.
 */
void handleEmptyPosition(Board *board, bool isPlayer1);

/**
 * @brief Handles the rotation of the selected ship on the board.
 * 
 * This function rotates the selected ship on the board, if possible, and updates its orientation.
 * 
 * @param board A pointer to the Board structure containing the current board state.
 * @param isPlayer1 A boolean indicating if it is player 1's board.
 */
void handleRotation(Board *board, bool isPlayer1);

/**
 * @brief Handles the completion of ship placement and transitions to the next game state.
 * 
 * This function finalizes the placement of the selected ship on the board and transitions
 * the game state accordingly.
 * 
 * @param board A pointer to the Board structure containing the current board state.
 * @param game A pointer to the Game structure containing the current game state and data.
 * @param isPlayer1 A boolean indicating if it is player 1's turn.
 * @param time A pointer to an integer representing the current timer value.
 */
void handleEnter(Board *board, Game *game, bool isPlayer1, int *time);

/**
 * @brief Handles the keyboard input during the SETUP or PLAYING state.
 * 
 * This function processes the scancode input during ship placement,
 * and delegates the actions accordingly.
 * 
 * @param board A pointer to the Board structure containing the current board state.
 * @param game A pointer to the Game structure containing the current game state and data.
 * @param isPlayer1 A boolean indicating if it is player 1's turn.
 * @param scancode The scancode of the key that was pressed.
 * @param time A pointer to an integer representing the current timer value.
 */
void handleScancode(Board *board, Game *game, bool isPlayer1, int scancode, int *time);
#endif // DEVICESCONTROLLER_H
