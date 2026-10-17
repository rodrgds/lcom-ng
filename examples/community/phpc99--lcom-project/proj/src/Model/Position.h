#ifndef _POSITION_H_
#define _POSITION_H_

#include <stdint.h>
#include <stdio.h>

/**
 * @file Position.h
 * @brief Defines Position structure for 2D coordinate representation.
 *
 * Position struct is a simple utility to represent x and y coordinates
 * on a 2D plane, for graphical elements.
 */

 /**
 * @struct Position
 * @brief Represents a 2D point using integer coordinates.
 *
 * The Position structure stores horizontal (x) and vertical (y) coordinates
 * using 16-bit signed integers, allowing negative positions if needed.
 */

typedef struct
{
    int16_t x; 
    int16_t y; 
} Position; 

#endif
