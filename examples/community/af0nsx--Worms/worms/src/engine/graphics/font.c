#include "font.h"

#include <stdlib.h>
#include "renderer.h"
#include "../../config.h"

struct Font {
    Sprite  *glyphs[FONT_GLYPH_COUNT];
    uint8_t  char_spacing;
};

// Returns glyph index for c, or -1 for unsupported characters.
// Lowercase is silently treated as uppercase.
static int char_to_index(char c) {
    if (c >= 'a' && c <= 'z') return FONT_IDX_A + (c - 'a');
    if (c >= 'A' && c <= 'Z') return FONT_IDX_A + (c - 'A');
    if (c >= '0' && c <= '9') return FONT_IDX_0 + (c - '0');
    if (c == ' ')             return FONT_IDX_SPACE;
    return -1;
}

Font *font_create(Sprite *const glyphs[FONT_GLYPH_COUNT], uint8_t char_spacing) {
    Font *f = malloc(sizeof(Font));
    if (f == NULL) return NULL;
    for (int i = 0; i < FONT_GLYPH_COUNT; i++)
        f->glyphs[i] = glyphs[i];
    f->char_spacing = char_spacing;
    return f;
}

void font_destroy(Font *f) {
    if (f == NULL) return;
    free(f);
}

int font_draw_string(const Font *f, const char *str, int x, int y) {
    if (f == NULL || str == NULL) return 1;
    int cx = x;
    for (const char *p = str; *p != '\0'; p++) {
        if (*p == ' ') {
            cx += FONT_SPACE_WIDTH + (int)f->char_spacing;
            continue;
        }
        int idx = char_to_index(*p);
        if (idx < 0 || f->glyphs[idx] == NULL) continue;
        if (renderer_draw_sprite(f->glyphs[idx], cx, y) != 0) return 1;
        cx += (int)sprite_get_width(f->glyphs[idx]) + (int)f->char_spacing;
    }
    return 0;
}

int font_measure_string(const Font *f, const char *str) {
    if (f == NULL || str == NULL) return 0;
    int width = 0;
    for (const char *p = str; *p != '\0'; p++) {
        if (*p == ' ') {
            width += FONT_SPACE_WIDTH + (int)f->char_spacing;
            continue;
        }
        int idx = char_to_index(*p);
        if (idx < 0 || f->glyphs[idx] == NULL) continue;
        width += (int)sprite_get_width(f->glyphs[idx]) + (int)f->char_spacing;
    }
    if (width > 0) width -= (int)f->char_spacing;
    return width;
}
