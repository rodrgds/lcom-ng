/**
 * @file react_mode_view.h
 * @brief Interface for rendering the react mode game view and related UI elements.
 */

#ifndef _LCOM_REACT_MODE_VIEW_H_
#define _LCOM_REACT_MODE_VIEW_H_

#include <lcom/lcf.h>
#include "view/view.h"

/**
 * @brief Draws the frame for React Mode.
 * 
 * Renders the screen for React Mode. It updates the background color before and after
 * the reaction time. It also draws instructions based on the current player mode.
 * 
 * @param elapsed_seconds Number of seconds that have passed since the mode started.
 * @param target_seconds Number of seconds to wait before changing the background color.
 */
void draw_react_mode_frame(int elapsed_seconds, int target_seconds);

#endif /* _LCOM_REACT_MODE_VIEW_H_ */
