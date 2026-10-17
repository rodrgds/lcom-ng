/**
 * @file mouseModel.h
 * @brief Mouse model data structure and function declarations
 * 
 * This header file defines the Mouse structure and declares functions
 * for creating, managing, and destroying mouse objects in the game.
 */

#ifndef _MOUSEMODEL_H_
#define _MOUSEMODEL_H_

#include "../position.h"
#include "../Sprite/sprite.h"
#include "../XPMS/mouse.xpm"
#include <stdint.h>
#include <stdio.h>
#include <minix/sysutil.h>

/**
 * @brief Mouse structure representing the mouse cursor
 * 
 * This structure contains the mouse position and its associated sprite
 * for rendering the mouse cursor on screen.
 */
typedef struct Mouse{
    Position pos;       // Current position of the mouse cursor
    Sprite* sprite;     // Sprite used to render the mouse cursor
} Mouse; 

/**
 * @brief Creates a new mouse object
 * 
 * @param x Initial X coordinate for the mouse
 * @param y Initial Y coordinate for the mouse
 * @return Mouse object with initialized position and sprite
 */
Mouse createMouse( uint16_t x, uint16_t y);

/**
 * @brief Destroys a mouse object and frees its resources
 * 
 * @param m Mouse object to be destroyed
 */
void destroyMouse(Mouse m);

#endif
