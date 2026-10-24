#ifndef FONT_H
#define FONT_H

#include <stdint.h>

/** Width of one bitmap font glyph before scaling. */
#define FONT_WIDTH 5

/** Height of one bitmap font glyph before scaling. */
#define FONT_HEIGHT 7

/** Horizontal spacing between glyphs before scaling. */
#define FONT_SPACING 1

/**
 * @brief Draws one 5x7 bitmap font character.
 *
 * The glyph is drawn to the graphics back buffer. Each font bit becomes a
 * scale x scale rectangle, clipped to the screen.
 *
 * @param c Character to draw. Unsupported characters use a fallback glyph.
 * @param x Top-left x coordinate.
 * @param y Top-left y coordinate.
 * @param color Color encoded for the current VBE mode.
 * @param scale Integer scale factor. A value of 0 draws nothing.
 */
void font_draw_char(char c, int x, int y, uint32_t color, uint8_t scale);

/**
 * @brief Draws a null-terminated string with the bitmap font.
 *
 * Characters are drawn left to right using FONT_WIDTH and FONT_SPACING for
 * horizontal advance.
 *
 * @param text Null-terminated string to draw.
 * @param x Top-left x coordinate of the first character.
 * @param y Top-left y coordinate.
 * @param color Color encoded for the current VBE mode.
 * @param scale Integer scale factor. A value of 0 draws nothing.
 */
void font_draw_text(const char *text, int x, int y, uint32_t color, uint8_t scale);

/**
 * @brief Computes the rendered width of a text string.
 *
 * @param text Null-terminated string.
 * @param scale Integer font scale.
 * @return Width in pixels.
 */
int font_text_width(const char *text, uint8_t scale);

/**
 * @brief Draws text horizontally centered around a given x coordinate.
 *
 * @param text Null-terminated string to draw.
 * @param center_x X coordinate of the text center.
 * @param y Top-left y coordinate.
 * @param color Color encoded for the current VBE mode.
 * @param scale Integer font scale.
 */
void font_draw_text_centered(const char *text, int center_x, int y, uint32_t color, uint8_t scale);

/**
 * @brief Draws a signed integer using the bitmap font.
 *
 * @param number Number to draw.
 * @param x Top-left x coordinate of the first digit/sign.
 * @param y Top-left y coordinate.
 * @param color Color encoded for the current VBE mode.
 * @param scale Integer scale factor. A value of 0 draws nothing.
 */
void font_draw_number(int number, int x, int y, uint32_t color, uint8_t scale);

#endif
