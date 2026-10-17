#ifndef _WALL_H_
#define _WALL_H_

#include <stdint.h>

/** @defgroup wall wall
 * @{
 *
 * Functions related to the wall
 */

/**
 * @brief Struct for the wall
 */
typedef struct {
  int16_t x; /*!< horizontal coordinate of the wall (top-left corner) */
  int16_t y; /*!< vertical coordinate of the wall (top-left corner) */
  uint8_t w; /*!< width of the wall */
  uint8_t h; /*!< height of the wall */
} Wall;

/**
 * @brief Wall constructor
 *
 * @param x horizontal coordinate of the wall (top-left corner)
 * @param y vertical coordinate of the wall (top-left corner)
 * @param w width of the wall
 * @param h height of the wall
 * @return Constructed wall on success, NULL otherwise
 */
Wall *construct_wall(int16_t x, int16_t y, uint8_t w, uint8_t h);

/**
 * @brief Moves wall up
 *
 * @param wall pointer to the wall to be moved
 * @return Return 0 upon success and non-zero otherwise
 */
int move_wall_up(Wall *wall);

/**
 * @brief Moves wall down
 *
 * @param wall pointer to the wall to be moved
 * @param y_max maximum vertical coordinate of the wall (screen limit/boundary)
 * @return Return 0 upon success and non-zero otherwise
 */
int move_wall_down(Wall *wall, uint16_t y_max);

/**
 * @brief Wall destructor
 *
 * @param wall pointer to the wall to be destroyed
 */
void destroy_wall(Wall *wall);

/**@}*/

#endif /* _WALL_H_ */
