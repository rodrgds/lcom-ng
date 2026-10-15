#ifndef _MENU_DRAW_H_
#define _MENU_DRAW_H_

#include <stdint.h>
#include "../game/menu_state.h"
#include "../init.h"
#include "status_bar.h"

/**
 * @file menu_draw.h
 * @brief Declares main menu hit-testing and drawing helpers.
 */

/**
 * @brief Resolve a screen point to a main menu option.
 *
 * @param x Horizontal point coordinate.
 * @param y Vertical point coordinate.
 * @return Menu option under the point, or MENU_OPTION_NONE.
 */
MenuOption ui_menu_hit_test(int x, int y);

/**
 * @brief Draw the main menu screen.
 *
 * @param menu Menu state to render.
 * @param assets Shared assets used for drawing.
 * @param status Status bar values to render.
 * @return 0 on success, non-zero on failure.
 */
int ui_menu_draw(const menu_t *menu, const Assets *assets, const StatusBarInfo *status);

#endif /* _MENU_DRAW_H_ */
