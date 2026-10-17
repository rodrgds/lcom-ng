#ifndef _WALL_VIEW_H_
#define _WALL_VIEW_H_

#include "../model/wall.h"

/** @defgroup wall_view wall_view
 * @{
 *
 * Functions related to the wall view
 */

/**
 * @brief Wall drawer
 *
 * @param wall pointer to the wall to be drawn
 * @return Return 0 upon success and non-zero otherwise
 */
int draw_wall(Wall *wall);

/**@}*/

#endif /* _WALL_VIEW_H_ */
