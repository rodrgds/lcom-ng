#ifndef SCENE_H_
#define SCENE_H_

#include "ascii_map.h"

/** @defgroup scene scene
 * @{
 * Functions for controlling the scenes.
 */

#define kColorBlack 0

#define kVideoMode 0x105

#define kDefaultWidth	1024
#define kDefaultHeight	768

/// Struct for the Screen Resolution
typedef struct kScreenRes {
    int w;
    int h;
} res;

/// Struct for Rectangles (and its positioning)
typedef struct kRect {
    int x;
    int y;
    int w;
    int h;
} rect;

/// Enum of the internal interrupt modes
typedef enum {
    kInterruptModeInvalid = 0,
    kInterruptModeMainMenu = 1,
    kInterruptModeEndGame = 2,
    kInterruptModeInGame = 3
} kInterruptMode;

/// Current screen resolution
static res screenRes = {
    .w = kDefaultWidth,
    .h = kDefaultHeight
};

/// Board rectangle and its positioning
static rect boardRect = {
    .x = kDefaultWidth / 10,
    .y = kDefaultHeight / 10,
    .w = kDefaultHeight / 20 * 8,
    .h = kDefaultHeight / 10 * 8
};

/// Player 2 board rectangle and its positioning
static rect boardRectSecond = {
    .x = kDefaultWidth / 2 + kDefaultWidth / 10,
    .y = kDefaultHeight / 10,
    .w = kDefaultHeight / 20 * 8,
    .h = kDefaultHeight / 10 * 8
};

/// Next piece rectangle and its positioning (1P)
static rect nextPieceRect = {
    .x = kDefaultWidth / 10 * 2 + kDefaultHeight / 20 * 8,
    .y = kDefaultHeight / 10,
    .w = kDefaultHeight / 10 * 2,
    .h = kDefaultHeight / 10 * 2
};

/// Next piece rectangle and its positioning (2P)
static rect nextPieceRectMP = {
    .x = kDefaultWidth / 10 + kDefaultHeight / 20 * 9,
    .y = kDefaultHeight / 10,
    .w = kDefaultHeight / 10 * 2,
    .h = kDefaultHeight / 10 * 2
};

/// Bounds for the scoring text
static rect scoreBounds = {
    .x = kDefaultWidth / 10 * 2 + kDefaultHeight / 20 * 8 + kDefaultHeight / 20,
    .y = kDefaultHeight / 10 * 4 + kDefaultHeight / 50,
    .w = 0,
    .h = 0
};

/// Score rectangle and its positioning
static rect scoreRect = {
    .x = kDefaultWidth / 10 * 2 + kDefaultHeight / 20 * 8,
    .y = kDefaultHeight / 10 * 4,
    .w = kDefaultHeight / 10 * 2,
    .h = kDefaultHeight / 20
};

/// Bounds for the time text
static rect timeBounds = {
    .x = kDefaultWidth / 10 * 2 + kDefaultHeight / 20 * 8 + kDefaultHeight / 20 + 8,
    .y = kDefaultHeight / 10 * 5 + kDefaultHeight / 50,
    .w = 0,
    .h = 0
};

/// Time rectangle and its positioning
static rect currTimeRect = {
    .x = kDefaultWidth / 10 * 2 + kDefaultHeight / 20 * 8,
    .y = kDefaultHeight / 10 * 5,
    .w = kDefaultHeight / 10 * 2,
    .h = kDefaultHeight / 20
};

/// Bounds for the endgame score.
static rect endGameScoreBounds = {
    .x = kDefaultWidth / 2,
    .y = kDefaultHeight / 2,
    .w = 0,
    .h = 0
};

/**
 * @brief Does the initialization job for all the scenes (in this case, for the program itself).
 */

void scene_init();

/**
 * @brief Resets the interrupts.
 */

void scene_intReset();

/**
 * @brief Terminates the program.
 */

void scene_terminate();

/**
 * @brief Sets the internal interrupt mode.
 */

void scene_setInterruptMode(kInterruptMode mode);

/**
 * @brief Initializes the GPU for video mode.
 */

void scene_initVideoGraphics();

/**
 * @brief Initializes the timer.
 * 
 * @return 0 on success, undefined if otherwise.
 */

int scene_initTimer();

/**
 * @brief Initializes the keyboard.
 *
 * @return 0 on success, undefined if otherwise.
 */

int scene_initKeyboard();

/**
 * @brief Initializes the mouse.
 *
 * @return 0 on success, undefined if otherwise.
 */

int scene_initMouse();

/**
 * @brief Draws the next piece on screen.
 *
 * @param players The number of players ingame.
 */

void scene_drawNext(int players);

/**
 * @brief Cleans the screen (fills with black).
 */

void scene_cleanScreen();

/**
 * @brief Exits the GPU graphical mode, and gives control back to the shell.
 */

void scene_exitVideoGraphics();

/**
 * @brief Draws the single-player end game screen.
 */

void scene_drawEndGameSingle();

/**
 * @brief Draws the multiplayer end game screen.
 */

void scene_drawEndGameMulti();

/**
 * @brief Draws the main menu.
 *
 * @return 0 on success, undefined if otherwise.
 */

int scene_drawMainMenu();

/**
 * @brief Gets the representation size of a board piece.
 * @return rect representating a board piece.
 */

rect _boardPieceRepSize();

/**
 * @brief Gets the representation size of the next piece.
 * @return rect representating the next piece.
 */

rect _nextPieceRepSize();

/**
 * @brief Convenience function for drawing rectangles.
 * 
 * Calls vg_draw_rect, and converts a rect variable to .x and .y coordinates.
 *
 * @return 0 on success, undefined if otherwise.
 */

unsigned int _drawRect(rect toDraw, unsigned long color, unsigned int fill);

/**@}*/

#endif /* SCENE_H_ */
