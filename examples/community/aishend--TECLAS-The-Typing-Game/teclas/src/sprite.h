/**
 * @file sprite.h
 * @brief XPM sprite loading and rendering.
 */
#ifndef _TECLAS_SPRITE_H_
#define _TECLAS_SPRITE_H_

#include <lcom/lcf.h>

/**
 * @brief A loaded XPM sprite with optional alpha support.
 */
typedef struct {
    uint8_t *colors;
    xpm_image_t img;
    bool has_alpha;
} Sprite;

/**
 * @brief Load a sprite from an XPM map.
 *
 * @param xpm XPM map source data.
 * @return Pointer to the allocated Sprite, or NULL on failure.
 */
Sprite *sprite_load(xpm_map_t xpm);

/**
 * @brief Load a sprite from an XPM map with alpha channel support.
 *
 * @param xpm XPM map source data.
 * @return Pointer to the allocated Sprite, or NULL on failure.
 */
Sprite *sprite_load_alpha(xpm_map_t xpm);

/**
 * @brief Draw a sprite at the given screen position.
 *
 * @param s Pointer to the sprite.
 * @param x X coordinate on screen.
 * @param y Y coordinate on screen.
 */
void sprite_draw(Sprite *s, uint16_t x, uint16_t y);

/**
 * @brief Draw a sprite with custom foreground and background colors.
 *
 * @param s Pointer to the sprite.
 * @param x X coordinate on screen.
 * @param y Y coordinate on screen.
 * @param fg Foreground color (applied to non-transparent pixels).
 * @param bg Background color (applied when has_bg is true).
 * @param has_bg Whether to draw the background color.
 */
void sprite_draw_colored(Sprite *s, uint16_t x, uint16_t y, uint32_t fg, uint32_t bg, bool has_bg);

/**
 * @brief Free all resources held by a sprite.
 *
 * @param s Pointer to the sprite to destroy.
 */
void sprite_destroy(Sprite *s);

#endif
