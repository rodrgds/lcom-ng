#ifndef _FONT_H_
#define _FONT_H_

#include <stdint.h>
#include "game/sprite.h"

/**
 * @file font.h
 * @brief Declares bitmap font loading, measuring, and drawing helpers.
 */

/** @brief Number of digit glyphs supported by the font. */
#define FONT_NUM_DIGITS 10

/** @brief Number of lowercase letter glyphs supported by the font. */
#define FONT_NUM_LETTERS 26

/** @brief Number of symbol glyphs supported by the font. */
#define FONT_NUM_SYMBOLS 2

/** @brief Total number of glyphs stored by Font. */
#define FONT_NUM_GLYPHS (FONT_NUM_DIGITS + FONT_NUM_LETTERS + FONT_NUM_SYMBOLS)

/** @brief Glyph index of the colon symbol. */
#define FONT_COLON_INDEX (FONT_NUM_DIGITS + FONT_NUM_LETTERS)

/** @brief Glyph index of the slash symbol. */
#define FONT_SLASH_INDEX (FONT_COLON_INDEX + 1)

/**
 * @brief Loaded fixed-width bitmap font.
 */
typedef struct {
  /** @brief Loaded glyph sprites. */
  sprite_t glyphs[FONT_NUM_GLYPHS];

  /** @brief Width assigned to each rendered character. */
  uint16_t char_width;

  /** @brief Number of glyph slots available in glyphs. */
  uint32_t num_chars;

  /** @brief Number of glyphs successfully loaded. */
  uint32_t loaded_chars;
} Font;

/**
 * @brief Load all glyph sprites for a bitmap font.
 *
 * @param font Font object to initialize.
 * @param char_width Width to use when measuring and advancing text.
 * @return 0 on success, non-zero on failure.
 */
int font_create(Font *font, uint16_t char_width);

/**
 * @brief Destroy all glyph sprites owned by a font.
 *
 * @param font Font to destroy.
 */
void font_destroy(Font *font);

/**
 * @brief Convert a supported character into a glyph index.
 *
 * @param c Character to map.
 * @return Glyph index, or -1 if the character is unsupported.
 */
int font_glyph_index(char c);

/**
 * @brief Measure text width at the font's normal scale.
 *
 * @param font Font used for measurement.
 * @param text Null-terminated text to measure.
 * @return Width in pixels.
 */
uint16_t font_text_width(const Font *font, const char *text);

/**
 * @brief Measure text width at an integer scale.
 *
 * @param font Font used for measurement.
 * @param text Null-terminated text to measure.
 * @param scale Integer scale factor.
 * @return Width in pixels.
 */
uint16_t font_scaled_text_width(const Font *font, const char *text, uint8_t scale);

/**
 * @brief Measure text width when rendered at half character advance.
 *
 * @param font Font used for measurement.
 * @param text Null-terminated text to measure.
 * @return Width in pixels.
 */
uint16_t font_half_text_width(const Font *font, const char *text);

/**
 * @brief Draw scaled text using the glyphs' original colors.
 *
 * @param font Font used for drawing.
 * @param text Null-terminated text to draw.
 * @param x Left coordinate of the text.
 * @param y Top coordinate of the text.
 * @param scale Integer scale factor.
 * @return 0 on success, non-zero on failure.
 */
int font_draw_scaled_text(const Font *font, const char *text, uint16_t x, uint16_t y, uint8_t scale);

/**
 * @brief Draw scaled text using a replacement color.
 *
 * @param font Font used for drawing.
 * @param text Null-terminated text to draw.
 * @param x Left coordinate of the text.
 * @param y Top coordinate of the text.
 * @param scale Integer scale factor.
 * @param color Color used for visible glyph pixels.
 * @return 0 on success, non-zero on failure.
 */
int font_draw_scaled_text_color(const Font *font, const char *text, uint16_t x, uint16_t y, uint8_t scale, uint32_t color);

/**
 * @brief Draw text using half-width character advance and a replacement color.
 *
 * @param font Font used for drawing.
 * @param text Null-terminated text to draw.
 * @param x Left coordinate of the text.
 * @param y Top coordinate of the text.
 * @param color Color used for visible glyph pixels.
 * @return 0 on success, non-zero on failure.
 */
int font_draw_half_text_color(const Font *font, const char *text, uint16_t x, uint16_t y, uint32_t color);

#endif /* _FONT_H_ */
