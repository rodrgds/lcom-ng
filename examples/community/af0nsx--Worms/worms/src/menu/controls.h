#ifndef CONTROLS_H
#define CONTROLS_H

#include <stdbool.h>
#include <stdint.h>

typedef enum {
    CONTROLS_ACTION_NONE = 0,
    CONTROLS_ACTION_BACK,
} ControlsAction;

/**
 * @brief Draws the controls screen.
 */
void controls_draw(void);

/**
 * @brief Updates controls-screen navigation.
 * @param mx Current mouse X position.
 * @param my Current mouse Y position.
 * @param lb_click True when the left mouse button was clicked this frame.
 * @param confirm True when Enter or Space confirms the selected action.
 * @return Selected controls action.
 */
ControlsAction controls_update(int16_t mx, int16_t my, bool lb_click, bool confirm);

#endif /* CONTROLS_H */
