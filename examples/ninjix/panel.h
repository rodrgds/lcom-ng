/**
 * @file panel.h
 * @brief UI panel drawing -- renders a stylised bordered panel region.
 * @ingroup group_ui
 *
 * Draws a 9-patch-style panel with dark wood-textured borders and a warm
 * parchment fill, used for the sidebar, shop cards, and info boxes.
 */

#ifndef PROJ_PANEL_H
#define PROJ_PANEL_H

#include <stdint.h>

#include "video_gr_ext.h"

/**
 * @brief Draw a 9-patch-style panel with dark borders and parchment fill.
 *
 * Used for the sidebar, shop cards, and info boxes.  The panel is drawn
 * as a filled rectangle with textured borders sampled from the panel sprite.
 *
 * @param x       Top-left X coordinate.
 * @param y       Top-left Y coordinate.
 * @param width   Panel width in pixels.
 * @param height  Panel height in pixels.
 * @return 0 on success.
 */
int panel_draw(uint16_t x, uint16_t y, uint16_t width, uint16_t height);

/**
 * @brief Draw the panel into a sprite at the given offset.
 *
 * Same visual output as @ref panel_draw, but renders into the supplied
 * vg_sprite_t so the result can be cached and blitted later. The sprite
 * must already be large enough to hold the panel at (x, y).
 *
 * @param sprite  Destination sprite (must be non-NULL with allocated pixels).
 * @param x       Top-left X coordinate inside the sprite.
 * @param y       Top-left Y coordinate inside the sprite.
 * @param width   Panel width in pixels.
 * @param height  Panel height in pixels.
 * @return 0 on success.
 */
int panel_draw_to_sprite(vg_sprite_t *sprite, uint16_t x, uint16_t y,
                         uint16_t width, uint16_t height);

#endif
