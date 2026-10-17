/** 
 * @file main_viewer.h
 * @brief Header file for @ref main_viewer.c
 * 
 * This file declares all the functions to be used for drawing the sprites of the game
 */


#ifndef _MAIN_VIEWER_PROJ_H_
#define _MAIN_VIEWER_PROJ_H_

#include <lcom/lcf.h>
#include <stdio.h>
#include "animations.h"
#include "../controller/main_controller.h"
#include "../model/structs.h"
#include "static_images.h"

/**
 * @brief Draws the arena and all it includes
 * 
 * Starts by drawing the visible section of the arena by calling vg_draw_image_section32(). It then draws all the enemies by calling draw_enemies() and the player with vg_draw_image32(). Then draws the hearts, the health status, the mouse and the timer, in this order. <br>
 * All the entities inside the arena have relative positions, which means the screen position in which they are drawn is their relative position minus the arena position.
 * 
 * @param arena The pointer to the arena to draw
 * @return 0 in case of success, 1 otherwise
 */
int (draw_arena)(arena* arena);

/**
 * @brief Draws the menu and all it includes
 * 
 * Draws the background images of the menu if it exists and then the options.
 * 
 * @param menu The pointer to the menu to draw
 * @return 0 in case of success, 1 otherwise
 */
int (draw_menu)(menu* menu);

/**
 * @brief Draw all the active enemies in the arena
 * 
 * Iterates through the list of active enemies and calls vg_draw_image32() to draw each one, converting their relative position to an absolute position.
 * 
 * @param arena The pointer to the arena to draw on
 * @return 0 in case of success, 1 otherwise
 */
int (draw_enemies)(arena* arena);

/**
 * @brief Draw all the active attacks in the arena
 * 
 * Iterates through the list of active attacks and calls vg_draw_image32() to draw each one, converting their relative position to an absolute position.
 * 
 * @param arena The pointer to the arena to draw on
 * @return 0 in case of success, 1 otherwise
 */
int (draw_attacks)(arena* arena);

/**
 * @brief Draw all the active hearts in the arena
 * 
 * Iterates through the list of active hearts and calls vg_draw_image32() to draw each one, converting their relative position to an absolute position.
 * 
 * @param arena The pointer to the arena to draw on
 * @return 0 in case of success, 1 otherwise
 */
int (draw_hearts)(arena* arena);


#endif /* _MAIN_VIEWER_PROJ_H_ */
