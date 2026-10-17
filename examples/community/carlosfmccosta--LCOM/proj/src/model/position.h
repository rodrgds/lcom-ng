/**
 * @file position.h
 * @brief Structure for representing a 2D position.
 *
 * This header defines the position_t struct for storing x and y coordinates.
 */
#ifndef __POSITION_H
#define __POSITION_H

#include <stdint.h>

/**
 * @brief Represents a 2D position with x and y coordinates.
 */
typedef struct {
  int16_t x;              /**< X coordinate */
  int16_t y;             /**< Y coordinate */
} position_t;

#endif //__POSITION_H
