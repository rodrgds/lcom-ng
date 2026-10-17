#include <lcom/lcf.h>
#include "video_gr.h"
#include "keyboard.h"
#include "Bitmap.h"
#include "timer.h"
#include "menus.h"


/**
 * @file tetris.h
 * @brief Project Module with functions to manage the game itself
 */


/** @defgroup tetris tetris
 * @{
 * Functions to handle the game itself
 */


// board["y"]["x"]
int board[22][15];			//!< Game board (game area)
bool status[22][15];		//!< Status board (occupied or not)

/**
 * @brief Struct to store bitmaps for numbers
 */
typedef struct {
	Bitmap* number0;		//!< Bitmap pointer to number 0
	Bitmap* number1;		//!< Bitmap pointer to number 1
	Bitmap* number2;		//!< Bitmap pointer to number 2
	Bitmap* number3;		//!< Bitmap pointer to number 3
	Bitmap* number4;		//!< Bitmap pointer to number 4
	Bitmap* number5;		//!< Bitmap pointer to number 5
	Bitmap* number6;		//!< Bitmap pointer to number 6
	Bitmap* number7;		//!< Bitmap pointer to number 7
	Bitmap* number8;		//!< Bitmap pointer to number 8
	Bitmap* number9;		//!< Bitmap pointer to number 9
} numbers;

/**
 * @brief Struct to store the blocks, info about speed, points, current, next and on hold
 * piece, numbers and background
 */
typedef struct {
	Bitmap* b1;		//!< Blue block
	Bitmap* b2;		//!< Green block
	Bitmap* b3;		//!< Light Blue block
	Bitmap* b4;		//!< Orange block
	Bitmap* b5;		//!< Purple block
	Bitmap* b6;		//!< Red block
	Bitmap* b7;		//!< Yellow Block

	int speed;		//!< Game speed (1 to 3)

	int points;		//!< Current game points

	int current_piece;	//!< Current piece
	int next_piece;			//!< Next piece to fall
	int on_hold;				//!< On hold piece

	numbers numbers;		//!< Struct for the numbers

	Bitmap* background;	//!< Pointer to background bitmap
} tetris;



/**
 * @brief GET Method for status board
 */
int* getStatusBoard();

/**
 * @brief Starts a match, with a desired speed (1 to 3)
 * @return tetris struct with all the info
 */
tetris startGame(int speed);

/**
 * @brief Prints the current points on the screen
 */
int printPoints(tetris* game);

/**
 * @brief Fills the game board with zeros
 * Zeros represent blank (unoccupied spots)
 */
int fillWithZeros();

/**
 * @brief Displays the game board
 * It maps a number to a color, or more precisely, a block
 */
int printBoard(tetris* game);

/**
 * @brief Displays the next piece on the apropriate area
 */
int printNext(tetris* game);

/**
 * @brief Displays the on hold piece on the apropriate area
 */
int printOnHold(tetris* game);

/**
 * @brief Function to drop a piece
 * @return 0 for normal drop
 * @return 1 for early game-over
 * @return 2 for game-over
 */ 
int dropPiece(tetris* game, menus_t* menus);

/**
 * @brief Function to desintegrate complete horizontal lines
 * This function looks up for completed lines and erases them, dropping the
 * upper layers
 */
int desintegrateLines(tetris* game);

/**
 * @brief This is the main function
 */
int tetris_start(tetris* game, menus_t* menus);

/**
 * @brief This functions updates the game status
 * Basically, it assembles the game:
 * - reset double buffer
 * - draw background
 * - print points
 * - print next
 * - print on hold
 */
int updateFrame(tetris* game);

/**
 * @brief Checks near colision with left
 */
bool checkNearColisionLeft(tetris* game, int blocks, int pos[][2]);

/**
 * @brief Checks near colision with right
 */
bool checkNearColisionRight(tetris* game, int blocks, int pos[][2]);

/**
 * @brief Checks near colision with bottom
 */
bool checkNearColisionBottom(tetris* game, int blocks, int pos[][2]);

/**
 * @brief Checks colisions
 * @return 1 piece stuck on right side
 * @return 2 piece stuck on left side
 * @return 3 piece stuck between two pieces
 * @return 4 piece colides with right
 * @return 5 piece colides with left
 * @return 6 piece colides with bottom
 * @return 0 no colision
 */
int checkColision(tetris* game, int blocks, int pos[][2]);

/**
 * @brief Pause Menu function
 * @return 1 to exit game
 * @return 2 to keep playing
 */
int pauseMenu(menus_t* menus);

/**
 * @brief Game Over function
 */
int gameOverMenu(tetris* game, menus_t* menus);
