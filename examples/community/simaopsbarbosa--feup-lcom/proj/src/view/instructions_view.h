/**
 * @file instructions_view.h
 * @brief Interface for rendering the instructions screen and related UI elements.
 */

#ifndef _LCOM_INSTRUCTIONS_VIEW_H_
#define _LCOM_INSTRUCTIONS_VIEW_H_

#include <lcom/lcf.h>
#include "view/view.h"

/**
 * @brief Draws the instructions screen frame.
 * 
 * Renders the instructions for both game modes, explaning how to play them, and an option to return to the menu.
 */
void draw_instructions_frame();

#endif /* _LCOM_INSTRUCTIONS_VIEW_H_ */
