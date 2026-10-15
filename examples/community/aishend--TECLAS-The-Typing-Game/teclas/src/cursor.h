/**
 * @file cursor.h
 * @brief Mouse cursor management.
 */
#ifndef _TECLAS_CURSOR_H_
#define _TECLAS_CURSOR_H_

#include <lcom/lcf.h>

/**
 * @brief Initialize the mouse cursor.
 *
 * Sets up the cursor sprite and initial position.
 */
void cursor_init();

/**
 * @brief Move the cursor by a delta amount.
 *
 * @param dx Change in the x position.
 * @param dy Change in the y position.
 */
void cursor_move(int16_t dx, int16_t dy);

/**
 * @brief Draw the cursor on the screen.
 */
void cursor_draw();

/**
 * @brief Destroy cursor resources.
 *
 * Releases any memory or sprite resources used by the cursor.
 */
void cursor_destroy();

/**
 * @brief Current cursor x coordinate.
 */
extern int16_t cursor_x;

/**
 * @brief Current cursor y coordinate.
 */
extern int16_t cursor_y;

#endif
