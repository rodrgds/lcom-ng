#pragma once

/** @defgroup mouse_proj mouse_proj
 * @{
 * Functions for manipulating the mouse inside the project
 */

/**
 * @brief      Initialize project mouse structure. 
 */
void init_mouse();

/**
 * @brief      Destroy mouse structure and free memory used by it
 */
void destroy_mouse();

/**
 * @brief      Draw mouse to its current coordinates on screen
 */
void draw_mouse();

/**
 * @brief      Update mouse informations from mouse packet
 *
 * @param[in]  pak   { mouse packet used for information }
 */
void update_mouse(struct packet pak);
