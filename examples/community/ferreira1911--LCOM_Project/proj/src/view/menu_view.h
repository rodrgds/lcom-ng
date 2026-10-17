#ifndef MENU_VIEW_H
#define MENU_VIEW_H

#include <lcom/lcf.h>

#include "model/menu.h"

/**
 * @brief Draws a menu item if it is visible.
 *
 * Renders the XPM image of the given MenuItem at its position.
 *
 * @param item Pointer to the MenuItem to draw
 */
void (draw_menu_item)(const MenuItem *item);

#endif
