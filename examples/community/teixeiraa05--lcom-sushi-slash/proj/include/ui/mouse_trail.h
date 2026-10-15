#ifndef _MOUSE_TRAIL_H_
#define _MOUSE_TRAIL_H_

#include "../../lab4/mouse.h"

/**
 * @file mouse_trail.h
 * @brief Declares slash trail update and drawing helpers.
 */

/**
 * @brief Record the latest mouse state into the slash trail.
 *
 * @param mouse_state Current mouse state.
 */
void mouse_trail_update(const MouseGameState *mouse_state);

/**
 * @brief Draw the current slash trail.
 *
 * @param mouse_state Current mouse state used for slash visibility.
 * @return 0 on success, non-zero on failure.
 */
int mouse_trail_draw(MouseGameState *mouse_state);

#endif /* _MOUSE_TRAIL_H_ */
