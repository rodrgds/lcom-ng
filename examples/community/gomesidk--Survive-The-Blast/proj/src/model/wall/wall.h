#ifndef __WALL_H
#define __WALL_H

#include <lcom/lcf.h>
#include <stddef.h>
#include <stdlib.h>
#include "sprite.h"

/**
 * @file wall.h
 * @brief Wall management in the game
 * 
 * This module provides functions to create, manage, and draw walls in the game.
 */

struct wall_imp;
typedef struct wall_imp Wall;

/**
 * @brief Returns the X coordinate of the wall
 * 
 * @param w Pointer to the wall
 * @return The X coordinate of the wall
 */
uint8_t get_wall_Xpos(Wall *w);

/**
 * @brief Returns the Y coordinate of the wall
 * 
 * @param w Pointer to the wall
 * @return The Y coordinate of the wall
 */
uint8_t get_wall_Ypos(Wall *w);

/**
 * @brief Returns whether the wall is destroyable
 * 
 * @param w Pointer to the wall
 * @return true if the wall is destroyable, false otherwise
 */
bool is_wall_destroyable(Wall *w);

/**
 * @brief Returns whether the wall is active
 * 
 * @param w Pointer to the wall
 * @return true if the wall is active, false otherwise
 */
bool is_wall_active(Wall *w);

/**
 * @brief Sets the active state of the wall
 * 
 * @param w Pointer to the wall
 * @param active The new active state of the wall
 */
void set_wall_active(Wall *w, bool active);

/**
 * @brief Returns the sprite of the wall
 * 
 * @param w Pointer to the wall
 * @return Pointer to the sprite of the wall
 */
Sprite* get_wall_sprite(Wall *w);

/**
 * @brief Creates a new wall with the specified sprite
 * 
 * @param x The x-coordinate of the wall
 * @param y The y-coordinate of the wall
 * @param img The sprite for the wall
 * @param is_destroyable Whether the wall is destroyable
 * @param active Whether the wall is active
 * @return Pointer to the created wall, or NULL if the pixmap is invalid
 */
Wall *create_wall(uint8_t x, uint8_t y, Sprite *img, bool is_destroyable, bool active);

/**
 * @brief Frees the memory allocated for the wall
 * 
 * @param w Pointer to the wall to be destroyed
 */
void destroy_wall(Wall *w);

/**
 * @brief Draws the wall on the screen, only if the wall is active
 * 
 * @param w Pointer to the wall to be drawn
 * @param x_initial_grid The initial X coordinate of the grid
 * @param y_initial_grid The initial Y coordinate of the grid
 * @param grid_square_width The width of the grid square
 */
void draw_wall(Wall *w, uint16_t x_initial_grid, uint16_t y_initial_grid, uint16_t grid_square_width);

#endif /* __WALL_H */

