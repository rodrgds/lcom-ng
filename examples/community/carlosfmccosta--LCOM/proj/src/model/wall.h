/**
 * @file wall.h
 * @brief Interface for wall entity management.
 *
 * This header provides the structure and functions for creating, drawing, and destroying walls,
 * as well as collision detection and loading walls from level data.
 */
#ifndef __WALL_H
#define __WALL_H

#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include "sprite.h"
#include "assets/wall_sprite.h"

/**
 * @brief Sprite of the wall
 */
struct wall;
typedef struct wall wall_t;

/**
 * @brief Array of pointers to walls and count of walls.
 */
typedef struct {
    wall_t **walls; /**< Array of wall pointers */
    int count;      /**< Number of walls */
} wall_array_t;

/**
 * @brief Initializes a wall at the specified position.
 * 
 * @param start_x X coordinate of the wall's start position.
 * @param start_y Y coordinate of the wall's start position.
 * @return Pointer to the created wall, or NULL on failure.
 */
wall_t* wall_init(int16_t start_x, int16_t start_y);

/**
 * @brief Frees all resources associated with the wall.
 * 
 * @param wall Pointer to the wall to destroy.
 */
void destroy_wall(wall_t *wall);

/**
 * @brief Draws the wall sprite.
 * 
 * @param wall Pointer to the wall to draw.
 * @return 0 on success, non-zero on failure.
 */
int wall_draw(const wall_t *wall); 

/**
 * @brief Checks if a sprite collides with any wall in the given array.
 * 
 * @param sp Pointer to the sprite to check collision against.
 * @param wall_arr Pointer to the array of walls.
 * @param dx Optional output parameter to store collision offset in X direction.
 * @param dy Optional output parameter to store collision offset in Y direction.
 * @return true if collision detected, false otherwise.
 */
bool collides_with_any_wall(sprite_t *sp, wall_array_t *wall_arr, int *dx, int *dy);

/**
 * @brief Gets the width of the wall sprite.
 * 
 * @param wall Pointer to the wall.
 * @return Width in pixels.
 */
uint16_t get_wall_width(wall_t *wall);

/**
 * @brief Gets the height of the wall sprite.
 * 
 * @param wall Pointer to the wall.
 * @return Height in pixels.
 */
uint16_t get_wall_height(wall_t *wall);

/**
 * @brief Gets the position of the wall.
 * 
 * @param wall Pointer to the wall.
 * @return Pointer to the wall's position structure.
 */
position_t * get_wall_pos(wall_t *wall);

/**
 * @brief Loads walls from a level array of strings.
 * 
 * @param level Array of strings representing the level layout.
 * @param rows Number of rows in the level.
 * @param tile_width Width of each tile.
 * @param tile_height Height of each tile.
 * @return wall_array_t struct containing an array of wall pointers and count.
 */
wall_array_t load_walls_from_array(const char **level, int rows, int tile_width, int tile_height);

/**
 * @brief Gets the sprite associated with the wall.
 * 
 * @param wall Pointer to the wall.
 * @return Pointer to the wall's sprite.
 */
sprite_t* get_wall_sprite(wall_t *wall);

/**
 * @brief Sample level layout using '#' as walls.
 */
extern const char *level1[];

/**
 * @brief Number of rows in the sample level1.
 */
extern const int LEVEL1_ROWS;

#endif //__WALL_H
