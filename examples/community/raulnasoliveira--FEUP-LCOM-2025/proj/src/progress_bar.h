#ifndef PROGRESS_BAR_H
#define PROGRESS_BAR_H

#include <stdint.h>
#include "game.h"
#include "graphics.h"
#include "Sprite.h"

/**
 * @file progress_bar.h
 * @brief  Progress‐bar sprite management for game screens.
 *
 * This module provides functions to create, draw, update, and destroy
 * a progress‐bar sprite used in reaction‐time and memory games.
 */

/**
 * @brief   Creates a new progress‐bar sprite.
 * @param   x           Top‐left X coordinate where the bar will be drawn.
 * @param   y           Top‐left Y coordinate where the bar will be drawn.
 * @param   sprite_case Index or type of bar to create (e.g., empty, partial, full).
 * @return  Pointer to an allocated Sprite object, or NULL on failure.
 */
Sprite *create_pro_sprite(int x, int y, int sprite_case);

/**
 * @brief   Draws the progress bar in its current state.
 * @param   sp           Pointer to the progress‐bar Sprite.
 * @param   buffer       Target pixel buffer (e.g., double_buffer).
 * @param   sprite_case  Index specifying which bar segment to draw.
 */
void draw_pro_bar(Sprite *sp, uint8_t *buffer, int sprite_case);

/**
 * @brief   Draws the progress result (final state) on screen.
 * @param   sp           Pointer to the progress‐bar Sprite.
 * @param   buffer       Target pixel buffer.
 * @param   sprite_case  Index specifying the final result graphic.
 */
void draw_pro_result(Sprite *sp, uint8_t *buffer, int sprite_case);

/**
 * @brief   Frees all resources used by a progress‐bar sprite.
 * @param   sp  Pointer to the Sprite to destroy.
 */
void destroy_pro_bar(Sprite *sp);

#endif


