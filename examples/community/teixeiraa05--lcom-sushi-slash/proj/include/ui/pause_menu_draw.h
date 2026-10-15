#ifndef _PAUSE_MENU_DRAW_H_
#define _PAUSE_MENU_DRAW_H_

#include <stdint.h>
#include "../game/pause_menu_state.h"
#include "../init.h"
#include "status_bar.h"

/**
 * @file pause_menu_draw.h
 * @brief Declares pause menu hit-testing and drawing helpers.
 */

/**
 * @brief Resolve a screen point to a pause menu option.
 *
 * @param x Horizontal point coordinate.
 * @param y Vertical point coordinate.
 * @return Pause option under the point, or PAUSE_OPTION_NONE.
 */
PauseOption ui_pause_menu_hit_test(int x, int y);

/**
 * @brief Draw the pause menu overlay.
 *
 * @param menu Pause menu state to render.
 * @param assets Shared assets used for drawing.
 * @param status Status bar values to render.
 * @return 0 on success, non-zero on failure.
 */
int ui_pause_menu_draw(const pause_menu_t *menu, const Assets *assets, const StatusBarInfo *status);

#endif /* _PAUSE_MENU_DRAW_H_ */
