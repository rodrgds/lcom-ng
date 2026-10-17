/**
 * This module contains all functions that handle the pieces and is movement in the board in the three modes
 * Jorge Vale: 10%
 * Telmo Barros: 90%
 * Relative Weight: 12%
 */

#ifndef __GAME_H
#define __GAME_H

#include "piece.h"
#include <stdio.h>

/** @defgroup Game Game
 * @{
 * Functions for manipulating game
 */

//Game constants
#define ONE_PLAYER_BOARD_X					332
#define ONE_PLAYER_BOARD_Y					70
#define ONE_PLAYER_RELATIVE_NEXT_PIECE_X	420
#define ONE_PLAYER_RELATIVE_NEXT_PIECE_Y	90
#define RELATIVE_COUNTER_Y					0
#define RELATIVE_NEXT_STRING_Y				40
#define RELATIVE_POINTS_STRING_Y			220
#define RELATIVE_POINTS_Y					260
#define RELATIVE_LINES_STRING_Y				360
#define RELATIVE_LINES_Y					400
#define RELATIVE_WAITING_LINES_STRING_Y		500
#define RELATIVE_WAITING_LINES_Y			540

#define TWO_PLAYER_BOARD_X					332
#define TWO_PLAYER_BOARD_Y					70
#define TWO_PLAYER_RELATIVE_NEXT_PIECE_X	420
#define TWO_PLAYER_RELATIVE_NEXT_PIECE_Y	90

#define BOARD_RELATIVE_MIDDLE_X				150

#define INIT_FALL_DELAY						40

/** @name States of the game */
/** @{ */
typedef enum {
	FALL,
	MOVE_RIGHT,
	MOVE_LEFT,
	ROTATE,
	SWAP_PIECES,
	REACH_END,
	DO_NOTHING,
	GAME_OVER
} State;
/** @} end of States of the game */

/** @name Events of keyboard related to piece movement, rotation and swap */
/** @{ */
typedef enum kbd_game_event {
	LEFTKEY_DOWN,
	RIGHTKEY_DOWN,
	UPKEY_DOWN,
	DOWNKEY_DOWN,
	SPACEBAR_DOWN,
	NOKEY
} kbd_game_event;
/** @} end of Events of keyboard related to piece movement, rotation and swap */

/** @name Events of timer related to piece falling */
/** @{ */
typedef enum timer_game_event {
	FALL_TICK,
	NO_TICK
} timer_game_event;
/** @} end of Events of timer related to piece falling */

/** @name Events of mouse related to piece movement, rotation and swap */
/** @{ */
typedef enum mouse_game_event {
	MOUSE_RIGHT,
	MOUSE_LEFT,
	MOUSE_LEFT_BTN,
	MOUSE_MIDDLE_BTN,
	MOUSE_RIGHT_BTN,
	MOUSE_STOPPED
} mouse_game_event;
/** @} end of Events of mouse related to piece movement, rotation and swap */

/** @name Board structure */
/** @{
 *
 * Board structure that stores pieces already played
 */
typedef struct {
	unsigned int x,y;	// board position
	unsigned int width, height;	// board dimensions
	uint16_t * map;	// pointer to board image, first with edges only
} Board;
/** @} end of Board structure */

/** @name Game structure */
/** @{
 *
 * Game structure
 */
typedef struct {
	unsigned int game_mode;	// game mode (singleplayer, multiplayer or battle)
	Piece * actual_piece;	// actual piece that is falling
	Piece * next_piece;	// next piece stored that will fall
	State state, last_state;	// current game state
	kbd_game_event kbd_event, last_kbd_event;	// current keyboard game event
	mouse_game_event mouse_event;	// current mouse game event
	unsigned int fall_delay;	// time delay between "automatic" falls
	timer_game_event timer_event;	// current timer game event
	Board board;	// board that stores pieces already played
	unsigned int pieces_already_swapped;	// current keyboard mouse event
	unsigned int points;	// points obtained
	unsigned int lines;	// lines completed
	unsigned int lines_received;	// lines in queue to be added (multiplayer mode only)
} Game;
/** @} end of Game structure */

/**
 * @brief Game constructor
 *
 * @param mode game_mode to create (singleplayer, multiplayer or battle)
 *
 * @return A pointer to the game
 */
Game * new_game(unsigned int mode);

/**
 * @brief Updates game state given the current events
 *
 * @param game Game to change
 * @param event_type Device that changed event (0 kbd, 1 mouse, 2 timer)
 */
void update_gamestate(Game * game, unsigned int event_type);

/**
 * @brief Updates game(actual_piece position) based on current game state
 *
 * @param game Game to change
 */
void update_game(Game * game);
/**
 * @brief Draws the board, actual and next piece, points and lines in the video buffer
 *
 * @param game Game to draw
 */
void draw_game(Game * game);

/**
 * @brief Deletes the game by releasing the allocated memory
 *
 * @param game Game to destroy
 */
void delete_game(Game * game);

/**
 * @brief Add a piece to board
 *
 * @param piece Piece to add to board (commonly actual_piece
 * @param board Board to add the piece
 */
void add_piece(Piece * piece, Board * board);

/**
 * @brief Remove finished lines
 *
 * @param board Board to remove the finished lines
 */
int remove_finished_lines(Board * board);

/**
 * @brief Rotate piece clockwise
 *
 * @param piece Piece to rotate clockwise
 * @param board Board to check if the piece can rotate
 */
void rotate_piece(Piece * piece, Board * board);

/**
 * @brief Swap pieces map and dimensions, used to change actual and next piece
 *
 * @param actual Actual piece
 * @param next Next piece
 * @param board Board to place next piece
 */
void swap_pieces(Piece * actual, Piece * next, Board * board);

/**
 * @brief Used in multiplayer mode to add received lines in the bottom of the board
 *
 * @param board Board to place received lines
 * @param lines Number of lines to add
 */
int add_lines_received(Board * board, unsigned int lines);

/**@}*/

#endif /* __GAME_H */
