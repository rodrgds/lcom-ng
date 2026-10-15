#ifndef _EXPLOSION_H
#define _EXPLOSION_H

#include <lcom/lcf.h>
#include <stddef.h>
#include <stdlib.h>
#include "animatedSprite.h"


/**
 * @file explosion.h
 * @brief Explosion management in the game
 * 
 * This module provides functions to create, manage, and draw explosions in the game.
 * Explosions are used to indicate the destruction of walls or other objects.
 */

struct explosion_imp;
typedef struct explosion_imp Explosion;


/**
 * @brief Returns the X coordinate of the explosion
 * 
 * @param b Pointer to the
  * @return The X coordinate of the explosion
  */
uint8_t get_explosion_Xpos(Explosion *b);

/**
 * @brief Returns the Y coordinate of the explosion
 * 
 * @param b Pointer to the
 * @return The Y coordinate of the explosion
 */
uint8_t get_explosion_Ypos(Explosion *b);

/**
 * @brief Checks if the explosion is active
 *
 * @param b Pointer to the explosion to be checked
 * @return true if the explosion is active, false otherwise
 */
bool is_explosion_active(Explosion *b);

/**
 * @brief Sets the explosion's active status
 *
 * @param b Pointer to the explosion to be modified
 * @param active The new active status of the explosion
 */
void set_explosion_active(Explosion *b, bool active);


/**
 * @brief Creates a new explosion with the specified sprite
 *
 * @param x The x-coordinate of the explosion in the game grid
 * @param y The y-coordinate of the explosion in the game grid
 * @param img The sprite for the explosion
 * @return Pointer to the created explosion, or NULL if the pixmap is invalid
 */
Explosion *create_explosion(uint8_t x, uint8_t y, AnimSprite *img);


/**
 * @brief Frees the memory allocated for the explosion
 *
 * @param b Pointer to the explosion to be destroyed
 */
void destroy_explosion(Explosion *b); 

/**
 * @brief Draws the explosion on the screen, if active
 *
 * @param b Pointer to the explosion to be drawn
 * @param x_initial_grid The initial X coordinate of the grid
 * @param y_initial_grid The initial Y coordinate of the grid
 * @param grid_square_width The width of each grid square
 */
void draw_explosion(Explosion *b, uint16_t x_initial_grid, uint16_t y_initial_grid, uint16_t grid_square_width);

#endif /* __EXPLOSION_H */

