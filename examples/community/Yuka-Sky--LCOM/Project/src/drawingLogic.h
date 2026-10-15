#ifndef _DRAWING_LOGIC_H_
#define _DRAWING_LOGIC_H_

#include "mousekb/keyboard.h"
#include "mousekb/mouse.h"
#include "graphics/graphics.h"

#define MAX_X_GUESSING 600
#define MAX_Y_GUESSING 730

/**
 * @brief Switches between drawing and not drawing states using mouse_packet data.
 * 
 * @param mouse_packet Mouse event packet containing mouse button states and the x and y movement.
 */
void (update_drawing_state)(struct packet mouse_packet);

/**
 * @brief Draws a line in drawing screen using Bresenham's algorithm.
 * 
 * @param initial_x int of initial coordinate x of line.
 * @param initial_y int of initial coordinate y of line.
 * @param final_x int of final coordinate x of line.
 * @param final_y int of final coordinate y of line.
 * @param drawingScreen array of pixels representing the drawing screen area.
 * @return 0 on success or 1 if line exceeds drawing screen limits.
 */
int (update_drawing_screen)(int initial_x, int initial_y, int final_x, int final_y, uint32_t drawingScreen[]);

/**
 * @brief Handles mouse events in guessing screen including mouse coordinates, drawing state and clear drawing area functionality.
 * 
 * @param img a struct containing the images’s width, height and type information.
 * @param mouse_packet Mouse event packet containing mouse button states and the x and y movement.
 * @param x pointer to current mouse x coordinate.
 * @param y pointer to current mouse y coordinate.
 * @param drawingScreen array of pixels representing the drawing screen area.
 * @param c_mouse pointer for checking if mouse or draw area need to be redrawn (c_mouse > 0).
 * @return 0 on success.
 */
int (mouse_event_handler_guessing)(xpm_image_t img, struct packet mouse_packet, int* x, int* y, uint32_t drawingScreen[], int* c_mouse);

/**
 * @brief Resets drawing screen to initial colors (white for main area and background color for bottom part).
 * 
 * @param drawingScreen array of pixels representing the drawing screen area.
 * @return 0 on success.
 */
int (reset_drawing_area)(uint32_t drawingScreen[]);

#endif /*_DRAWING_LOGIC_H_*/
