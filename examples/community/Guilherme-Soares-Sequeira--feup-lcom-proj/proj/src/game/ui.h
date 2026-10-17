#ifndef UI_H
#define UI_H

#include <lcom/lcf.h>

#include "../devices/video/graphics.h"
#include "canvas.h"
#include "draw_buttons.h"
#include "draw_text.h"

/** @defgroup UI UI
 *  @{
 *
 * Constants, structures and functions used for programming our program's UI
 */

/**
 * @brief Draws the menu (background) and calls the function responsible for drawing the buttons
 *
 */
void(draw_menu)();

/**@}*/

#endif
