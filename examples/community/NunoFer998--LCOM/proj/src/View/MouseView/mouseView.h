/**
 * @file mouseView.h
 * @brief Mouse view header file
 * 
 * This header file defines the mouse view interface for rendering
 * the mouse cursor on the screen.
 */

#ifndef _MOUSEVIEW_H_
#define _MOUSEVIEW_H_

#include "../../Model/Mouse/mouseModel.h"
#include "../../Controller/Devices/VideoCard/video_graphics.h"

/**
 * @brief Draws the mouse cursor on the screen
 * 
 * This function renders the mouse cursor sprite at its current position.
 * 
 * @param m Mouse object containing position and sprite data
 * @return 0 on success, non-zero on error
 */
int drawMouse(Mouse m);

#endif
