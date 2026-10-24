#ifndef SPRITE_H
#define SPRITE_H

#include <lcom/lcf.h>
#include <stdint.h>
#include <stdbool.h>

/**
 * @brief Runtime representation of an XPM sprite.
 *
 * The pixel map is allocated by xpm_load() in sprite_load() and must be freed
 * with sprite_destroy(). transparent_color stores the mode-dependent color
 * value that should not be drawn.
 */
typedef struct {
  uint16_t width;
  uint16_t height;
  uint8_t bytes_per_pixel;
  uint32_t transparent_color;
  uint8_t *map;
  bool loaded;
} Sprite;

/**
 * @brief Loads an XPM map into a Sprite using the current graphics mode.
 *
 * The XPM is converted to the active VBE pixel format, so sprite drawing can
 * copy colors directly to the graphics back buffer.
 *
 * @param sprite Destination Sprite structure.
 * @param xpm Source XPM map.
 * @return 0 on success, non-zero otherwise.
 */
int sprite_load(Sprite *sprite, xpm_map_t xpm);

/**
 * @brief Draws a sprite at a top-left screen position.
 *
 * Transparent pixels are skipped and visible pixels are clipped to the screen.
 *
 * @param sprite Loaded sprite to draw.
 * @param x Top-left x coordinate.
 * @param y Top-left y coordinate.
 */
void sprite_draw(const Sprite *sprite, int x, int y);

/**
 * @brief Draws a sprite scaled by an integer factor.
 *
 * Each source pixel becomes a scale x scale rectangle. The input position is
 * still the top-left corner of the scaled sprite.
 *
 * @param sprite Loaded sprite to draw.
 * @param x Top-left x coordinate of the scaled sprite.
 * @param y Top-left y coordinate of the scaled sprite.
 * @param scale Integer scale factor. A value of 0 draws nothing.
 */
void sprite_draw_scaled(const Sprite *sprite, int x, int y, uint8_t scale);

/**
 * @brief Draws a sprite centered on a screen point without scaling.
 *
 * @param sprite Loaded sprite to draw.
 * @param center_x Center x coordinate.
 * @param center_y Center y coordinate.
 */
void sprite_draw_centered(const Sprite *sprite, int center_x, int center_y);

/**
 * @brief Draws a scaled sprite centered on a screen point.
 *
 * Useful for effects such as explosions, where the logical position is the
 * impact center rather than the top-left corner.
 *
 * @param sprite Loaded sprite to draw.
 * @param center_x Center x coordinate.
 * @param center_y Center y coordinate.
 * @param scale Integer scale factor. A value of 0 draws nothing.
 */
void sprite_draw_scaled_centered(const Sprite *sprite, int center_x, int center_y, uint8_t scale);

/**
 * @brief Frees the sprite pixel map and resets the Sprite structure.
 *
 * Safe to call with NULL or with an unloaded sprite.
 *
 * @param sprite Sprite to destroy.
 */
void sprite_destroy(Sprite *sprite);

#endif
