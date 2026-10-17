#pragma once
#include "Bitmap.h"
#include <stdint.h>

/** @defgroup obstacle obstacle
 * @{
 * Functions for manipulating obstacle objects
 */

/**
 * @brief      Represents one obstacle object
 */
struct Obstacle {
  int x; 			/**< x coordinate on screen */
  int y; 			/**< y coordinate on screen */
  int size_x;		/**< size of the obstacle in the x axis */
  int size_y;		/**< size of the obstacle in the y axis */
  Bitmap* obsBit;	/**< Bitmap corresponding to obstacle */
};

/**
 * @brief      Create a random obstacle in a random location
 *
 * @return     { obstacle created }
 */
struct Obstacle create_obj();

/**
 * @brief      Initialize obstacles structure
 */
void init_obstacles();

/**
 * @brief      Destroy obstacles structure and free memory used by it
 */
void destroy_obstacles();

/**
 * @brief      Draws obstacles sprites to screen.
 */
void draw_obstacles();
