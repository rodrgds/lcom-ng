#ifndef RENDERER_H
#define RENDERER_H

#include <stdint.h>
#include "sprite.h"

/**
 * @brief Provides a color for a visible masked pixel.
 *
 * @param col Pixel column inside the mask.
 * @param row Pixel row inside the mask.
 * @param run_depth Depth inside the current vertical solid mask run.
 * @param context Caller-owned context.
 * @return Pixel color to draw.
 */
typedef uint32_t (*RendererPixelColorFn)(int col, int row, int run_depth, void *context);

/**
 * @brief Clears the entire draw buffer with black.
 */
void renderer_clear(void);

/**
 * @brief Draws an XPM sprite at (x, y), skipping transparent pixels.
 * @param sprite Sprite created with create_sprite_xpm().
 * @return 0 on success, 1 on error.
 */
int renderer_draw_sprite(const Sprite *sprite, int x, int y);

/**
 * @brief Draws a sprite using a boolean mask. Only pixels where mask is 1 are drawn.
 * @param mask Array of mask_w * mask_h size.
 */
int renderer_draw_terrain(const Sprite *sprite, int x, int y, const uint8_t *mask, int mask_w, int mask_h);

/**
 * @brief Draws masked pixels using a caller-provided color function.
 *
 * The renderer only handles clipping and buffer writes; the caller decides the
 * color of each visible mask pixel.
 *
 * @param x Destination X.
 * @param y Destination Y.
 * @param mask Mask stored as [mask_w][mask_h].
 * @param mask_w Mask width.
 * @param mask_h Mask height.
 * @param color_fn Callback that returns the color for each visible mask pixel.
 * @param context User context passed to color_fn.
 * @return 0 on success, 1 on error.
 */
int renderer_draw_masked_pixels(int x, int y, const uint8_t *mask, int mask_w, int mask_h, RendererPixelColorFn color_fn, void *context);

/**
 * @brief Draws a solid color rectangle.
 */
int renderer_draw_rect(int x, int y, int w, int h, uint32_t color);

/**
 * @brief Flips the page: makes the draw buffer visible, switches to the other page.
 * Call this once per frame after all drawing functions.
 * @return 0 on success, 1 otherwise.
 */
int renderer_flip(void);

#endif /* RENDERER_H */
