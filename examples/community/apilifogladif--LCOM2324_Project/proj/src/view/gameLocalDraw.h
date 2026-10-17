#ifndef _LCOM_GAMELOCALDRAW_H_
#define _LCOM_GAMELOCALDRAW_H_

/**
 * @file gameLocalDraw.h
 * 
 * @brief Header file for local game drawing functions.
 */

#include "controllers/keyboard/kbd.h"
#include "controllers/mouse/mouse.h"
#include "controllers/video_graphics/graphics.h"
#include "controllers/timer/timer.h"
#include "controllers/macros.h"
#include "model/boats.h"
#include "model/game.h"
#include "assetDraw.h"

/**
 * @enum GAMESTATE
 * @brief Enumeration of possible game states.
 */
enum GAMESTATE {
    TURN1,
    TURN2,
    END,
};

/**
 * @brief Draws the local game state on the screen.
 */
void drawGameLocal();

/**
 * @brief Draws the game cursor on the screen.
 */
void drawGameCursor();

/**
 * @brief Draws the secret board for a specified player.
 * 
 * @param p : The player number (1 or 2).
 * @return bool : Returns true if the player has all boats sunk, otherwise false.
 */
bool drawPlayerSecretBoard(int p);

/**
 * @brief Draws the small secret board for a specified player.
 * 
 * @param p : The player number (1 or 2).
 */
void drawPlayerSmallSecretBoard(int p);

/**
 * @brief Draws a sunken boat on the screen.
 * 
 * @param boat : The boat to be drawn.
 */
void drawSunkenBoat(Boat boat);

/**
 * @brief Draws all sunken boats for a specified player.
 * 
 * @param p : The player number (1 or 2).
 */
void drawSunken(int p);

/**
 * @brief Draws a small version of a sunken boat on the screen.
 * 
 * @param boat : The boat to be drawn.
 */
void drawSmallSunkenBoat(Boat boat);

/**
 * @brief Draws all small sunken boats for a specified player.
 * 
 * @param p : The player number (1 or 2).
 */
void drawSmallSunken(int p);

#endif
