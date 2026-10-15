#ifndef DRAWER_H
#define DRAWER_H

#include "../drivers/drivers.h"
#include "../model/app/app.h"

/**
 * @defgroup drawer Drawer
 * @{
 * 
 * @brief Functions for drawing the game state and handling screen rendering.
 */

/**
 * @brief Draws the current state of the application.
 *
 * @details This function draws the various elements of the game based on the current state stored in the app structure.
 * 
 * @param app Pointer to the application structure.
 */
void draw_state(app_t *app);

/**
 * @brief Draws the entire screen.
 *
 * @details This function first checks if a redraw is necessary. If it is, it draws the background, the current game state,
 * the cursor, and then flips the screen buffer.
 * 
 * @param app Pointer to the application structure.
 */
void draw_screen(app_t *app);

/**
 * @brief Draws a set of buttons on the screen.
 *
 * @details This function iterates over the queue of buttons and draws each button at the specified position on the screen.
 * 
 * @param buttons Pointer to the queue of buttons to be drawn.
 */
void draw_button_set(queue_t *buttons);

/** @} */

#endif
