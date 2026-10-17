/**
 * @file squareModel.h
 * @brief Square model for minesweeper board elements
 * 
 * This header file defines the Square structure that represents
 * individual squares on the minesweeper game board, including
 * their properties and state information.
 */

#ifndef _SQUAREMODEL_H_
#define _SQUAREMODEL_H_

#include "../position.h"
#include "../Sprite/sprite.h"
#include <stdint.h>
#include <stdio.h>
#include <minix/sysutil.h>

/**
 * @brief Structure representing a single square on the minesweeper board
 * 
 * This structure contains all the necessary information for a minesweeper
 * square, including its position, visual properties, and game state.
 */
typedef struct Square{
    Position pos;           // Position of the square on the board
    uint32_t color;         // Current display color of the square
    uint32_t dirt_color;    // Color when square is revealed (dirt color)
    bool hasMine;           // Flag indicating if square contains a mine
    uint8_t nCloseMines;    // Number of mines in adjacent squares
    bool isFlagged;         // Flag indicating if square is marked by player
    bool full;              // Flag indicating if square has been revealed/dug
} Square; 

#endif


