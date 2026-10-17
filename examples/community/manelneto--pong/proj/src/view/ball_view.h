#ifndef _BALL_VIEW_H_
#define _BALL_VIEW_H_

#include "../model/ball.h"

/** @defgroup ball_view ball_view
 * @{
 *
 * Functions related to the ball view
 */

/**
 * @brief Ball drawer
 *
 * @param ball pointer to the ball to be drawn
 * @return Return 0 upon success and non-zero otherwise
 */
int draw_ball(Ball *ball);

/**@}*/

#endif /* _BALL_VIEW_H_ */
