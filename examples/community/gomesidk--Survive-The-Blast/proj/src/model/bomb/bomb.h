#ifndef __BOMB_H
#define __BOMB_H

#include <lcom/lcf.h>
#include <stddef.h>
#include <stdlib.h>
#include "animatedSprite.h"

/**
 * @file bomb.h
 * 
 * @brief Bomb management in the game
 * 
 * This module provides functions to create, manage, and draw bombs in the game.
 * It handles the bomb's position, active status, explosion status, and type.
 */

struct bomb_imp;
typedef struct bomb_imp Bomb;

typedef enum {
  NORMAL = 0,
  FULL_LINE = 1,
  CONSTRUCTIVE = 2
} BombType;

/**
 * @brief Returns the X coordinate of the bomb
 * 
 * @param b Pointer to the
  * @return The X coordinate of the bomb
  */
uint8_t get_bomb_Xpos(Bomb *b);

/**
 * @brief Returns the Y coordinate of the bomb
 * 
 * @param b Pointer to the
 * @return The Y coordinate of the bomb
 */
uint8_t get_bomb_Ypos(Bomb *b);

/**
 * @brief Checks if the bomb is active
 *
 * @param b Pointer to the bomb to be checked
 * @return true if the bomb is active, false otherwise
 */
bool is_bomb_active(Bomb *b);

/**
 * @brief Sets the bomb's active status
 *
 * @param b Pointer to the bomb to be modified
 * @param active The new active status of the bomb
 * @param type The type of the bomb
 */
void set_bomb_active(Bomb *b, bool active, BombType type);

/**
 * @brief Checks if the bomb has exploded
 *
 * @param b Pointer to the bomb to be checked
 * @return true if the bomb has exploded, false otherwise
 */
bool is_bomb_exploded(Bomb *b);

/**
 * @brief Gets the type of the bomb
 *
 * @param b Pointer to the bomb to be checked
 * @return The type of the bomb
 */
BombType get_bomb_type(Bomb *b);

/**
 * @brief Creates a new bomb with the specified sprite
 *
 * @param x The x-coordinate of the bomb in the game grid
 * @param y The y-coordinate of the bomb in the game grid
 * @param img The sprite for the bomb
 * @param active The initial active status of the bomb
 * @return Pointer to the created bomb, or NULL if the pixmap is invalid
 */
Bomb *create_bomb(uint8_t x, uint8_t y, AnimSprite *img, bool active);


/**
 * @brief Frees the memory allocated for the bomb
 *
 * @param b Pointer to the bomb to be destroyed
 */
void destroy_bomb(Bomb *b); 

/**
 * @brief Draws the bomb on the screen, if active
 *
 * @param b Pointer to the bomb to be drawn
 * @param x_initial_grid The initial X coordinate of the grid
 * @param y_initial_grid The initial Y coordinate of the grid
 * @param grid_square_width The width of each grid square
 */
void draw_bomb(Bomb *b, uint16_t x_initial_grid, uint16_t y_initial_grid, uint16_t grid_square_width);

#endif /* __BOMB_H */

