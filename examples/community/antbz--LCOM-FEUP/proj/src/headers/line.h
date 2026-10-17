#pragma once

#include <lcom/lcf.h>


/**
 * @file line.h
 * @brief line_t object definition and manipulation.
 * 
 * File containing the definition of and functions to manipulate
 * line_t struct objects.
*/


/**
 * @brief Sprite-like struct for lines.
 * 
 * Contains an array of coordinates that correspond to the points that make
 * up a line. Used for having a sprite to intersect with Fruit objects.
 */
typedef struct {
  uint16_t* points; /**< Array of positions (x,y) of the points of the line. */
  size_t size; /**< Size of points array (twice the number of points). */
} line_t;


/**
 * @brief line_t constructor.
 * 
 * Creates a new line_t equivalent to a line between two specified points.
 * 
 * @param x_i the starting point x-coordinate.
 * @param y_i the starting point y-coordinate.
 * @param x_f the end point x-coordinate.
 * @param y_f the end point y-coordinate.
 * @return The new line_t object.
 */
line_t make_line(uint16_t x_i, uint16_t y_i, uint16_t x_f, uint16_t y_f);


/**
 * @brief line_t destructor.
 * 
 * Destroys a line_t object, freeing up the space allocated to the points array.
 * 
 * @param line a pointer to the line_t to destroy.
 */
void destroy_line(line_t* line);
