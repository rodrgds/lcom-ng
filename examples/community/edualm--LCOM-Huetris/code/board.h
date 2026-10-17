#ifndef BOARD_H_
#define BOARD_H_

/*
 * Note to self:
 *  - x -> width
 *  - y -> height
 */

#define kBoardMaxWidth 10
#define kBoardMaxHeight 22

#define kEnabledPiece 14

/** @defgroup board board
 * @{
 * Functions for controlling the game boards
 */

/// Move Piece To...
typedef enum {
    kMovePosUp,
    kMovePosLeft,
    kMovePosDown,
    kMovePosRight
} kMovePos;

/// Keyboard Key Codes
typedef enum {
    kKeyboardKey1 = 0x02,
    kKeyboardKey2 = 0x03,
	kKeyboardKeyUpArrow = 0x48,
	kKeyboardKeyLeftArrow = 0x4B,
	kKeyboardKeyDownArrow = 0x50,
	kKeyboardKeyRightArrow = 0x4D,
    kKeyboardKeySpaceBar = 0x39,
	kKeyboardKeyEsc = 0x01
} kKeyboardKey;

/// Defines the Special Piece Types
typedef enum {
    kPieceTypeEnabled = 14
} kPieceType;

/// Defines the Boards
typedef enum {
    kBoardOne = 1,
    kBoardTwo = 2
} kBoard;

/**
 * @brief Checks for lines that needs to be cleared and clears them.
 *
 * @param b The board to check.
 * @return The number of lines cleared.
 */

unsigned int board_clearLines(kBoard b);

/**
 * @brief Checks for pieces ingame.
 *
 * @param b The board to check.
 * @return 1 if true, 0 if false.
 */

unsigned int board_pieceInGame(kBoard b);

/**
 * @brief Tries to move a piece (and performs the action if possible).
 *
 * @param b The board to check.
 * @return 1 if true, 0 if false.
 */

unsigned int board_movePiece(kBoard b, kMovePos pos);

/**
 * @brief Glues a piece to the board.
 *
 * @param b The board to check.
 * @return The number of 1x1 blocks glued.
 */

unsigned int board_gluePiece(kBoard b);

/**
 * @brief Spawns a new piece, if possible.
 *
 * @param b The board to check.
 * @return 0 on successful execution, 1 if otherwise.
 */

unsigned int board_spawnPiece(kBoard b);

/**
 * @brief Tries to rotate a piece (and performs the action if possible).
 *
 * @param b The board to check.
 * @return 0 on successful execution, 1 if otherwise.
 */

unsigned int board_rotatePiece(kBoard b);

/**
 * @brief Gets the color on the asked coordinates.
 *
 * @param b The board to check.
 * @param x The x coordinate.
 * @param y The y coordinate.
 * @return The color at the specified position.
 */

unsigned int board_getColorAt(kBoard b, unsigned int x, unsigned int y);

/**
 * @brief Gets the game score.
 *
 * @return The game score.
 */

unsigned int board_getScore();

/**
 * @brief The in-game timer handler.
 */

void board_timerHandler();

/**
 * @brief The in-game keyboard handler.
 */

void board_keyboardHandler();

/**
 * @brief The in-game mouse handler.
 */

void board_mouseHandler();

/**
 * @brief Cleans the boards.
 */

void _cleanBoards();

static unsigned int board[kBoardMaxHeight][kBoardMaxWidth] = {0};
static unsigned int board_p2[kBoardMaxHeight][kBoardMaxWidth] = {0};

/**@}*/

#endif /* BOARD_H_ */
