/**
 * @file selectorMine.h
 * @brief Selector mine model header file
 * 
 * This header file defines the SelectorMine structure and functions
 * for managing the mine selector sprite used in menu navigation.
 */

#ifndef _SELECTORMINE_H_
#define _SELECTORMINE_H_

#include "../position.h"
#include "../Sprite/sprite.h"
#include "../XPMs/mine.xpm"
#include <stdint.h>
#include <stdio.h>
#include <minix/sysutil.h>

// Initial position constants for the selector mine
#define INIT_POS_X 496      // Initial X position for selector mine
#define INIT_POS_Y 250      // Initial Y position for selector mine

/**
 * @brief Structure representing the selector mine sprite
 * 
 * This structure contains the position and sprite data for the
 * mine selector used to indicate the currently selected menu option.
 */
typedef struct SMine{
    Position pos;       // Current position of the selector mine
    Sprite* sprite;     // Sprite used to render the selector mine
} SMine; 

/**
 * @brief Creates a new selector mine object
 * 
 * @return SMine object with initialized sprite and default position
 */
SMine createSMine();

/**
 * @brief Destroys a selector mine object and frees its resources
 * 
 * @param mine Pointer to the selector mine to be destroyed
 */
void destroySMine( SMine* mine);

/**
 * @brief Sets the position of the selector mine
 * 
 * @param mine Pointer to the selector mine
 * @param x New X coordinate
 * @param y New Y coordinate
 */
void setSMinePosition( SMine* mine, uint16_t x, uint16_t y);

#endif
