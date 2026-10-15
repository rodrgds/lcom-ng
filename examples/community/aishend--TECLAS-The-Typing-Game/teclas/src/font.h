/**
 * @file font.h
 * @brief Bitmap font loading and rendering.
 */
#ifndef _TECLAS_FONT_H_
#define _TECLAS_FONT_H_

#include <lcom/lcf.h>
#include "sprite.h"

/**
 * @brief A bitmap font with per-character sprites.
 */
typedef struct {
    Sprite *chars[128];
    uint16_t char_width;
    uint16_t char_height;
} Font;

/**
 * @brief Width in pixels of each character cell.
 */
extern uint16_t char_width;

/**
 * @brief Height in pixels of each character cell.
 */
extern uint16_t char_height;

/**
 * @brief Get the global font instance.
 *
 * @return Pointer to the loaded Font, or NULL if not yet initialized.
 */
Font *font_get();

/**
 * @brief Draw a single character with custom colors.
 *
 * @param c ASCII character to draw.
 * @param x X coordinate on screen.
 * @param y Y coordinate on screen.
 * @param fg Foreground color.
 * @param bg Background color.
 * @param has_bg Whether to draw the background color.
 */
void font_draw_char_colored(char c, uint16_t x, uint16_t y, uint32_t fg, uint32_t bg, bool has_bg);

/**
 * @brief Draw a null-terminated string with custom colors.
 *
 * @param str String to draw.
 * @param x X coordinate on screen.
 * @param y Y coordinate on screen.
 * @param fg Foreground color.
 * @param bg Background color.
 * @param has_bg Whether to draw the background color.
 */
void font_draw_string_colored(const char *str, uint16_t x, uint16_t y, uint32_t fg, uint32_t bg, bool has_bg);

/**
 * @brief Draw a string centered horizontally on screen.
 *
 * @param str String to draw.
 * @param y Y coordinate on screen.
 * @param fg Foreground color.
 */
void font_draw_centered(const char *str, uint16_t y, uint32_t fg);

/**
 * @brief Get the pixel width of a rendered string.
 *
 * @param str Null-terminated string.
 * @return Width in pixels.
 */
uint16_t font_string_width(const char *str);

/**
 * @brief Free all resources held by the global font.
 */
void font_destroy();

#endif
