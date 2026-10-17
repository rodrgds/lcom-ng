/**
 * @file gameController.h
 * @brief Game controller header for minesweeper logic
 * 
 * This header file defines the main game controller functions and structures
 * for handling minesweeper game logic, including board generation,
 * bomb placement, and game state management.
 */

#ifndef _GAMECONTROLLER_H_
#define _GAMECONTROLLER_H_

#include <lcom/lcf.h>
#include <stdio.h>
#include <stdint.h>

#include "Controller/Devices/VideoCard/video_graphics.h"
#include "../../Model/Square/squareModel.h"
#include "../../View/GameView/gameView.h"

// Game configuration constants
#define NUMBER_OF_BOMBS 35        // Total number of mines on the board

// Color constants for board rendering
#define GREEN_SQUARE_1 0x1BB302   // First green square color
#define GREEN_SQUARE_2 0x1ECC02   // Second green square color  
#define DIRT_SQUARE_1 0xB48818    // First dirt square color
#define DIRT_SQUARE_2 0x936A00    // Second dirt square color

// Global game state variables
Square gameBoard[15][15];         // 15x15 minesweeper board
bool firstMove;                   // Flag indicating if this is the first move

/**
 * @brief Builds and initializes the game board
 */
void buildGameBoard();

/**
 * @brief Sets proximity alerts around a bomb position
 * 
 * @param bomb_col Column of the bomb
 * @param bomb_row Row of the bomb
 */
void setProximityAlert(uint8_t bomb_col, uint8_t bomb_row);

/**
 * @brief Checks if a position is restricted for bomb placement
 * 
 * @param pos_x Position X to check
 * @param pos_y Position Y to check  
 * @param x Reference X position
 * @param y Reference Y position
 * @return true if position is restricted, false otherwise
 */
bool positionIsRestricted(uint8_t pos_x, uint8_t pos_y, uint8_t x, uint8_t y);

/**
 * @brief Handles digging action at specified position
 * 
 * @param col Column to dig
 * @param row Row to dig
 */
void dig(uint8_t col, uint8_t row);

/**
 * @brief Deploys bombs on the board avoiding the first dig position
 * 
 * @param dig_x X position of first dig (to avoid)
 * @param dig_y Y position of first dig (to avoid)
 */
void deployBombs(uint8_t dig_x, uint8_t dig_y);

#endif
