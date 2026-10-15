#ifndef PAUSE_MENU_H
#define PAUSE_MENU_H

#include <stdbool.h>
#include <stdint.h>

typedef enum {
    PAUSE_ACTION_NONE = 0,
    PAUSE_ACTION_RESUME,
    PAUSE_ACTION_CONTROLS,
    PAUSE_ACTION_EXIT_MENU,
} PauseAction;

/**
 * @brief Draws the pause menu overlay.
 */
void pause_menu_draw(void);

/**
 * @brief Updates pause-menu selection and activation.
 * @param mouse_x Current mouse X position.
 * @param mouse_y Current mouse Y position.
 * @param lb_click True when the left mouse button was clicked this frame.
 * @param nav_delta Keyboard navigation: -1 up, 0 none, +1 down.
 * @param confirm True when Enter or Space confirms the selection.
 * @return Selected pause action.
 */
PauseAction pause_menu_update(int16_t mouse_x, int16_t mouse_y, bool lb_click,
                              int nav_delta, bool confirm);

#endif /* PAUSE_MENU_H */
