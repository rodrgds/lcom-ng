#ifndef MENU_H
#define MENU_H

#include <stdbool.h>
#include <stdint.h>

/**
 * @brief Actions the player can trigger from the main menu.
 */
typedef enum {
    MENU_ACTION_NONE = 0,
    MENU_ACTION_PLAY,
    MENU_ACTION_CONTROLS,
    MENU_ACTION_EXIT,
} MenuAction;

/**
 * @brief Draws the menu background.
 */
void menu_draw_bg(void);

/**
 * @brief Draws the main menu based on the current selection state.
 *        Call after renderer_clear(), before renderer_flip().
 */
void menu_draw(void);

/**
 * @brief Processes input for the menu and returns any triggered action.
 *
 * Keyboard and mouse share the same selection state. Mouse hover updates
 * the selected button; arrow keys move it; click or Enter/Space confirm it.
 *
 * @param mouse_x   Current mouse X position.
 * @param mouse_y   Current mouse Y position.
 * @param lb_click  True if the left button was just clicked this frame.
 * @param nav_delta Keyboard navigation: -1 = up, 0 = none, +1 = down.
 * @param confirm   True if Enter or Space was pressed this frame.
 * @return The triggered MenuAction, or MENU_ACTION_NONE.
 */
MenuAction menu_update(int16_t mouse_x, int16_t mouse_y, bool lb_click,
                       int nav_delta, bool confirm);

#endif /* MENU_H */
