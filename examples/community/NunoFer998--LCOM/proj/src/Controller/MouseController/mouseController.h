/**
 * @file mouseController.h
 * @brief Mouse controller header file
 * 
 * This header file defines the mouse controller interface for handling
 * mouse input, position updates, and game interactions in the minesweeper game.
 */

#ifndef _MOUSECONTROLLER_H_
#define _MOUSECONTROLLER_H_

#include <lcom/lcf.h>
#include <stdio.h>
#include <stdint.h>

#include "../Devices/Mouse_Keyboard/Mouse/mouse.h"
#include "../../Model/Mouse/mouseModel.h"
#include "../GameController/gameController.h"
#include "../../View/GameView/gameView.h"

// External variable declarations
extern bool firstMove;              // Flag indicating if this is the first move
extern uint8_t flags;              // Mouse packet flags
extern bool startedExplosionTimer; // Flag for explosion timing

/**
 * @brief Handles mouse packet data and processes mouse input
 * 
 * @param mouse Pointer to the mouse object to update
 */
void handlePacket(Mouse* mouse);

/**
 * @brief Updates mouse position based on movement data
 * 
 * @param mouse Pointer to the mouse object to update
 */
void updatePosition(Mouse* mouse);

/**
 * @brief Updates mouse X coordinate with boundary checking
 * 
 * @param mouse Pointer to the mouse object to update
 */
void updatePositionX(Mouse* mouse);

/**
 * @brief Updates mouse Y coordinate with boundary checking
 * 
 * @param mouse Pointer to the mouse object to update
 */
void updatePositionY(Mouse* mouse);

/**
 * @brief Gets the game board row corresponding to mouse position
 * 
 * @param mouse Pointer to the mouse object
 * @return Row index on the game board
 */
uint8_t getRowInGame(Mouse* mouse);

/**
 * @brief Gets the game board column corresponding to mouse position
 * 
 * @param mouse Pointer to the mouse object
 * @return Column index on the game board
 */
uint8_t getColInGame(Mouse* mouse);

/**
 * @brief Handles right mouse button click (flagging squares)
 * 
 * @param mouse Pointer to the mouse object
 */
void handleRightClick(Mouse* mouse);

/**
 * @brief Handles left mouse button click (digging squares)
 * 
 * @param mouse Pointer to the mouse object
 */
void handleLeftClick(Mouse* mouse);

#endif
