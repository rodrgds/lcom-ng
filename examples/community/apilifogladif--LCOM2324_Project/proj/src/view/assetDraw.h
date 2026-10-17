#ifndef _LCOM_ASSETDRAW_H_
#define _LCOM_ASSETDRAW_H_

/**
 * @file assetDraw.h
 * 
 * @brief This file contains the main draws for the game.
*/

#include "controllers/sprites/sprite.h"
#include "model/boats.h"
#include "model/game.h"
#include "gameLocalDraw.h"

/**
 * @brief Draws the board sprite on the screen.
 */
void drawBoard();

/**
 * @brief Draws the game board sprite on the screen.
 */
void drawGameBoard();

/**
 * @brief Draws the name sprite for player 1 on the screen.
 */
void drawNamePlayer1();

/**
 * @brief Draws the quit text sprite on the screen.
 */
void drawQuitText();

/**
 * @brief Draws the go to game text text sprite on the screen.
 */
void drawGoGameText();

/**
 * @brief Draws the go to next player text sprite on the screen.
 */
void drawNextPlayerText();

/**
 * @brief Draws the name sprite for player 2 on the screen.
 */
void drawNamePlayer2();

/**
 * @brief Draws the cursor sprite on the screen.
 */
void drawCursor();

/**
 * @brief Draws the hand cursor sprite on the screen.
 */
void drawHandCursor();

/**
 * @brief Draws the crosshair sprite on the screen.
 */
void drawCrosshair();

/**
 * @brief Draws the end game sprite based on the winning player.
 * 
 * @param p : The number representing the winning player (1 or 2).
 */
void drawEndGame(int p);

/**
 * @brief Draws the waiting sprite on the screen.
 */
void drawWaiting();

/**
 * @brief Draws the myTurn sprite on the screen.
 */
void drawMyTurn();

/**
 * @brief Draws the time left for player's turn to end.
 */
void drawTime();


#endif
