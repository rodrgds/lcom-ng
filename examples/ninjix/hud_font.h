/**
 * @file hud_font.h
 * @brief Bitmap HUD font -- glyph storage and text rendering.
 * @ingroup group_ui
 *
 * The HUD font consists of 45 pre-rendered glyph sprites (digits,
 * uppercase letters, and common punctuation).  Text is drawn with
 * integer scaling and optional colour tinting.
 */

#ifndef PROJ_HUD_FONT_H
#define PROJ_HUD_FONT_H

#include <stdbool.h>
#include <stdint.h>

#include "video_gr_ext.h"

#define HUD_FONT_GLYPH_COUNT 45

typedef struct {
  vg_sprite_t glyphs[HUD_FONT_GLYPH_COUNT];
  bool loaded;
} hud_font_t;

/**
 * @brief Load all HUD font glyph sprites from disk.
 * @param font  Font struct to fill.
 * @return 0 on success, non-zero if a glyph fails to load.
 */
int hud_font_load(hud_font_t *font);

/**
 * @brief Release all HUD font glyph sprites.
 * @param font  Font struct to tear down.
 */
void hud_font_unload(hud_font_t *font);

/**
 * @brief Draw a text string using the HUD font with default (white) colour.
 * @param font   Loaded HUD font.
 * @param text   Null-terminated string to draw (uppercase + digits).
 * @param x      Screen X coordinate of the first glyph.
 * @param y      Screen Y coordinate of the first glyph.
 * @param scale  Integer scale factor (1 = native glyph size).
 * @return 0 on success.
 */
int hud_font_draw_text(const hud_font_t *font, const char *text, int16_t x, int16_t y, uint16_t scale);

/**
 * @brief Draw a text string with a custom colour tint.
 * @param font   Loaded HUD font.
 * @param text   Null-terminated string to draw.
 * @param x      Screen X coordinate.
 * @param y      Screen Y coordinate.
 * @param scale  Integer scale factor.
 * @param color  RGB colour to tint the glyphs.
 * @return 0 on success.
 */
int hud_font_draw_text_colored(const hud_font_t *font, const char *text, int16_t x, int16_t y,
                               uint16_t scale, uint32_t color);

/**
 * @brief Return the pixel height of a single line of text at the given scale.
 * @param scale  Integer scale factor.
 * @return Line height in pixels.
 */
uint16_t hud_font_line_height(uint16_t scale);

#endif
