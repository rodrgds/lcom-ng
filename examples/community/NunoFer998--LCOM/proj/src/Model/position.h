/**
 * @file position.h
 * @brief Position structure definition
 * 
 * This header file defines a simple Position structure used throughout
 * the game to represent 2D coordinates for various game objects.
 */

#ifndef _POSITION_H_
#define _POSITION_H_
#include <lcom/lcf.h>
#include <stdio.h>

/**
 * @brief Structure representing a 2D position
 * 
 * This structure holds X and Y coordinates used to represent
 * positions of game objects on the screen.
 */
typedef struct Position{
    uint16_t x, y;  // X and Y coordinates
} Position; 

#endif
