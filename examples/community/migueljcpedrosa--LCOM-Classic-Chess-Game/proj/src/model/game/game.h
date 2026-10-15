/**
 * @file game.h
 * @brief This file contains the definition of the game module.
 * 
 * @see Game
 */

#ifndef _GAME_H_
#define _GAME_H_

#include "board.h"
#include "move.h"

/** 
 * @brief Represents a chess game.
 * 
 * This struct represents a chess game. It is composed of a board, a white player, a black player and a turn.
*/
typedef Color Turn;

/**
 * @brief Represents a chess game.
 * 
 * This struct represents a chess game. It is composed of a board, a white player, a black player and a turn, whose value equals the color of the player of the current turn.
 * 
 * @see Board
 * @see Player
 * @see Turn
 */
typedef struct {

    Turn turn;
    Board* board;
    Player* white_player;
    Player* black_player;

} Game;

extern char* game_result;
extern char* game_winner;

extern Piece* selected_piece;
extern Game* game;

/**
 * @brief Initializes a game.
 * 
 * This function creates a game and initializes it's board and players.
 * 
 * @param white_name the name of the player that will control the white pieces.
 * @param black_name the name of the player that will control the black pieces.
 * 
 * @return Returns the created game.
 */
void (create_game)(char white_name[], char black_name[]);

/**
 * @brief Destroys a game.
 * 
 * This function destroys a game and frees all the memory allocated to it.
 * 
 * @param game pointer to the game that will be destroyed.
 */
void (destroy_game)();

/**
 * @brief Switches the turn of the game.
 * 
 * This function switches the turn of the game.
 * 
 * @param game pointer to the game whose turn will be switched.
 */
void (switch_turn)(Game* game);

/**
 * @brief Returns the current player.
 * 
 * This function returns the player that is currently playing.
 * 
 * @param game pointer to the game.
 * 
 * @return Returns the current player.
 */
Player* (get_current_player)(Game* game);

/**
 * @brief Returns the waiting player.
 * 
 * This function returns the player that is currently waiting for their turn.
 * 
 * @param game pointer to the game.
 * 
 * @return Returns the waiting player.
 */
Player* (get_waiting_player)(Game* game);

/**
 * @brief Checks if the current player has any valid moves.
 * 
 * @param game pointer to the game.
 * @return Returns true if the current player has any valid moves, false otherwise.
 */
bool (can_move)(Game* game);

/**
 * @brief Checks if the current player is in check.
 * 
 * @param game pointer to the game.
 * @return Returns true if the current player is in check, false otherwise.
 */
bool (is_check)(Game* game);

/**
 * @brief Checks if the current player is in check mate.
 * 
 * @param game pointer to the game.
 * @return Returns true if the current player is in check mate, false otherwise.
 */
bool (is_check_mate)(Game* game);


/**
 * @brief Checks if the current player is in stale mate.
 * 
 * @param game pointer to the game.
 * @return Returns true if the current player is in stale mate, false otherwise.
 */
bool (is_stale_mate)(Game* game);

/**
 * @brief Retrieves the piece at the given position.
 * 
 * @param game pointer to the game.
 * @param x the x coordinate of the position.
 * @param y the y coordinate of the position.
 * @return Returns the piece at the given position.
 */
Piece* (getPiece)(Game* game, unsigned int x, unsigned int y);

/** 
 * @brief Executes a move within the given game.
 * 
 * This function executes a move within the given game.
 * It changes the pieces positions and statuses and updates the board.
 * This function calls the appropriate function to execute the move, depending on the type of the move.
 * 
 * @param game pointer to the game.
 * @param move the move that will be executed.
*/
void (execute_move)(Game* game, Move move);

/**
 * @brief Executes a normal move.
 * 
 * @param game pointer to the game.
 * @param move the move that will be executed.
 */
void (execute_normal)(Game* game, Move move);

/**
 * @brief Executes a capture move.
 * 
 * @param game pointer to the game.
 * @param move the move that will be executed.
 */
void (execute_capture)(Game* game, Move move);

/**
 * @brief Executes a castle move.
 * 
 * @param game pointer to the game.
 * @param move the move that will be executed.
 */
void (execute_castle)(Game* game, Move move);

/**
 * @brief Executes an en passant move.
 * 
 * @param game pointer to the game.
 * @param move the move that will be executed.
 */
void (execute_enPassant)(Game* game, Move move);

/**
 * @brief Undoes a move within the given game.
 * 
 * This function undoes a move within the given game.
 * It changes the pieces positions and statuses, and the board back to its previous state.
 * This function calls the appropriate function to undo the move, depending on the type of the move.
 * 
 * @param game pointer to the game.
 * @param move the move that will be undone.
*/
void (undo_move)(Game* game, Move move);

/**
 * @brief Undoes a normal move.
 * 
 * @param game pointer to the game.
 * @param move the move that will be undone.
 */
void (undo_normal)(Game* game, Move move);

/**
 * @brief Undoes a capture move.
 * 
 * @param game pointer to the game.
 * @param move the move that will be undone.
 */
void (undo_capture)(Game* game, Move move);

/**
 * @brief Undoes a castle move.
 * 
 * @param game pointer to the game.
 * @param move the move that will be undone.
 */
void (undo_castle)(Game* game, Move move);

/**
 * @brief Undoes an en passant move.
 * 
 * @param game pointer to the game.
 * @param move the move that will be undone.
 */
void (undo_enPassant)(Game* game, Move move);

/**
 * @brief Filters out all the invalid moves of the given piece.
 * 
 * This function filters out all the moves that put the current player in check.
 * 
 * @param game pointer to the game.
 * @param piece pointer to the piece whose moves will be filtered.
 * 
 * @return Returns true if the move is valid, false otherwise.
 */
void (filterMoves)(Game* game, Piece* piece);

/**
 * @brief Sets the moves of the given piece.
 * 
 * This function sets the moves of the given piece, with all possible moves.
 * 
 * @param game pointer to the game.
 * @param piece pointer to the piece whose moves will be set.
 * @param checkForCheck boolean value that indicates if the function is being run within a check verification.
 */
void (setMoves)(Game* game, Piece* piece, bool checkForCheck);

/**
 * @brief Sets the moves of the given pawn.
 * 
 * This function sets the moves of the given pawn, with all possible moves.
 * 
 * @param game pointer to the game.
 * @param piece pointer to the pawn whose moves will be set.
 */
void (setPawnMoves)(Game* game, Piece* piece);

/**
 * @brief Sets the moves of the given piece in a line.
 * 
 * This function sets the moves of the given piece in a line.
 * 
 * @param game pointer to the game.
 * @param piece pointer to the piece whose moves will be set.
 * @param increment the increment that will generate the line.
 */
void (setMovesInLine)(Game* game, Piece* piece, Position increment);

/**
 * @brief Sets the moves of the give bishop.
 * 
 * This function sets the moves of the given bishop, with all possible moves.
 * 
 * @param game pointer to the game.
 * @param piece pointer to the bishop whose moves will be set.
 */
void (setBishopMoves)(Game* game, Piece* piece);

/**
 * @brief Sets the moves of the given rook.
 * 
 * This function sets the moves of the given rook, with all possible moves.
 * 
 * @param game pointer to the game.
 * @param piece pointer to the rook whose moves will be set.
 */
void (setRookMoves)(Game* game, Piece* piece);

/**
 * @brief Sets the moves of the given queen.
 * 
 * This function sets the moves of the given queen, with all possible moves.
 * 
 * @param game pointer to the game.
 * @param piece pointer to the queen whose moves will be set.
 */
void (setQueenMoves)(Game* game, Piece* piece);

/**
 * @brief Sets the moves of the given knight.
 * 
 * This function sets the moves of the given knight, with all possible moves.
 * 
 * @param game pointer to the game.
 * @param piece pointer to the knight whose moves will be set.
 */
void (setKnightMoves)(Game* game, Piece* piece);

/**
 * @brief Sets the moves of the given king.
 * 
 * This function sets the moves of the given king, with all possible moves.
 * 
 * @param game pointer to the game.
 * @param piece pointer to the king whose moves will be set.
 * @param checkForCheck boolean value that indicates if the function is being run within a check verification.
 */
void (setKingMoves)(Game* game, Piece* king, bool checkForCheck);

#endif
