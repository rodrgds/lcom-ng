#ifndef FONT_H
#define FONT_H

#include <stdint.h>
#include "sprite.h"

/** Total glyphs: space + A-Z + 0-9 */
#define FONT_GLYPH_COUNT  37

/** Index constants — used by assets.c to fill the glyph array */
#define FONT_IDX_SPACE    0
#define FONT_IDX_A        1
#define FONT_IDX_Z        26
#define FONT_IDX_0        27
#define FONT_IDX_9        36

typedef struct Font Font;

/**
 * @brief Creates a Font from an array of glyph sprites.
 *
 * The caller owns the Sprite objects; font_destroy() does NOT free them —
 * assets_cleanup() is responsible for that. The pointer array is copied
 * into the Font struct.
 *
 * @param glyphs        Array of FONT_GLYPH_COUNT Sprite pointers indexed by
 *                      FONT_IDX_* constants. NULL entries are valid (glyph missing).
 * @param char_spacing  Horizontal gap in pixels between consecutive glyphs.
 * @return Pointer to the new Font, or NULL on allocation failure.
 */
Font *font_create(Sprite *const glyphs[FONT_GLYPH_COUNT], uint8_t char_spacing);

/**
 * @brief Frees the Font struct. Does NOT free the underlying Sprite objects.
 */
void font_destroy(Font *f);

/**
 * @brief Draws a null-terminated string at (x, y).
 *
 * Lowercase letters are treated as uppercase. Unsupported characters are
 * skipped. Space advances by FONT_SPACE_WIDTH + char_spacing.
 *
 * @return 0 on success, 1 on any draw error.
 */
int font_draw_string(const Font *f, const char *str, int x, int y);

/**
 * @brief Returns the pixel width a string would occupy when rendered.
 *        Useful for centering text within a button.
 */
int font_measure_string(const Font *f, const char *str);

#endif /* FONT_H */
