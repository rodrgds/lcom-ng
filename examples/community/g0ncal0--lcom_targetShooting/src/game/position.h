#ifndef _POSITION_H_
#define _POSITION_H_

#include <lcom/lcf.h>

/**
 * @struct Position
 * @brief Represents a position in a 2D coordinate system.
 */
typedef struct {
  int16_t x; /** The x-coordinate of the position. */
  int16_t y; /** The y-coordinate of the position. */
} Position;

/**
 * @enum Direction
 * @brief Represents the possible directions.
 */
typedef enum {
  LEFT,  /** Represents the left direction. */
  RIGHT, /** Represents the right direction. */
  UP,    /** Represents the up direction. */
  DOWN   /** Represents the down direction. */
} Direction;

#endif
