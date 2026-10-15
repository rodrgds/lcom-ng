#include "../include/font.h"

#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "../include/ui/ui.h"

#include "../assets/numbers/zero.xpm"
#include "../assets/numbers/one.xpm"
#include "../assets/numbers/two.xpm"
#include "../assets/numbers/three.xpm"
#include "../assets/numbers/four.xpm"
#include "../assets/numbers/five.xpm"
#include "../assets/numbers/six.xpm"
#include "../assets/numbers/seven.xpm"
#include "../assets/numbers/eight.xpm"
#include "../assets/numbers/nine.xpm"

#include "../assets/letters/a.xpm"
#include "../assets/letters/b.xpm"
#include "../assets/letters/c.xpm"
#include "../assets/letters/d.xpm"
#include "../assets/letters/e.xpm"
#include "../assets/letters/f.xpm"
#include "../assets/letters/g.xpm"
#include "../assets/letters/h.xpm"
#include "../assets/letters/i.xpm"
#include "../assets/letters/j.xpm"
#include "../assets/letters/k.xpm"
#include "../assets/letters/l.xpm"
#include "../assets/letters/m.xpm"
#include "../assets/letters/n.xpm"
#include "../assets/letters/o.xpm"
#include "../assets/letters/p.xpm"
#include "../assets/letters/q.xpm"
#include "../assets/letters/r.xpm"
#include "../assets/letters/s.xpm"
#include "../assets/letters/t.xpm"
#include "../assets/letters/u.xpm"
#include "../assets/letters/v.xpm"
#include "../assets/letters/w.xpm"
#include "../assets/letters/x.xpm"
#include "../assets/letters/y.xpm"
#include "../assets/letters/z.xpm"
#include "../assets/letters/colon.xpm"
#include "../assets/letters/slash.xpm"

#define FONT_GLYPH_WHITE_888 0xFFFFFF

int font_create(Font *font, uint16_t char_width) {
    if (font == NULL) return 1;
    if (char_width == 0) return 1;

    memset(font, 0, sizeof(*font));
    font->char_width = char_width;
    font->num_chars = FONT_NUM_GLYPHS;

    xpm_map_t char_xpms[FONT_NUM_GLYPHS] = {
        (xpm_map_t)zero_xpm, (xpm_map_t)one_xpm, (xpm_map_t)two_xpm, (xpm_map_t)three_xpm,
        (xpm_map_t)four_xpm, (xpm_map_t)five_xpm, (xpm_map_t)six_xpm, (xpm_map_t)seven_xpm,
        (xpm_map_t)eight_xpm, (xpm_map_t)nine_xpm,
        (xpm_map_t)a_xpm, (xpm_map_t)b_xpm, (xpm_map_t)c_xpm, (xpm_map_t)d_xpm,
        (xpm_map_t)e_xpm, (xpm_map_t)f_xpm, (xpm_map_t)g_xpm, (xpm_map_t)h_xpm,
        (xpm_map_t)i_xpm, (xpm_map_t)j_xpm, (xpm_map_t)k_xpm, (xpm_map_t)l_xpm,
        (xpm_map_t)m_xpm, (xpm_map_t)n_xpm, (xpm_map_t)o_xpm, (xpm_map_t)p_xpm,
        (xpm_map_t)q_xpm, (xpm_map_t)r_xpm, (xpm_map_t)s_xpm, (xpm_map_t)t_xpm,
        (xpm_map_t)u_xpm, (xpm_map_t)v_xpm, (xpm_map_t)w_xpm, (xpm_map_t)x_xpm,
        (xpm_map_t)y_xpm, (xpm_map_t)z_xpm,
        (xpm_map_t)colon_xpm,
        (xpm_map_t)slash_xpm,
    };

    for (uint32_t i = 0; i < font->num_chars; i++) {
        if (sprite_load(&font->glyphs[i], char_xpms[i]) != 0) {
            printf("[font] failed to load glyph %u\n", i);
            font_destroy(font);
            return 1;
        }
        font->loaded_chars++;
    }
    return 0;
}

void font_destroy(Font *font) {
    if (font == NULL) return;

    uint32_t loaded_chars = font->loaded_chars;
    if (loaded_chars > FONT_NUM_GLYPHS) {
        loaded_chars = FONT_NUM_GLYPHS;
    }

    for (uint32_t i = 0; i < loaded_chars; i++) {
        sprite_destroy(&font->glyphs[i]);
    }

    memset(font, 0, sizeof(*font));
}

int font_glyph_index(char c) {
    if (c >= '0' && c <= '9') {
        return c - '0';
    } else if (c >= 'a' && c <= 'z') {
        return FONT_NUM_DIGITS + (c - 'a');
    } else if (c >= 'A' && c <= 'Z') {
        return FONT_NUM_DIGITS + (c - 'A');
    } else if (c == ':') {
        return FONT_COLON_INDEX;
    } else if (c == '/') {
        return FONT_SLASH_INDEX;
    } else {
        return -1;
    }
}

uint16_t font_text_width(const Font *font, const char *text) {
    uint16_t width = 0;

    if (font == NULL || text == NULL) return 0;

    for (const char *p = text; *p != '\0'; p++) {
        width += font->char_width;
    }

    return width;
}

uint16_t font_scaled_text_width(const Font *font, const char *text, uint8_t scale) {
    return font_text_width(font, text) * scale;
}

uint16_t font_half_text_width(const Font *font, const char *text) {
    return font_text_width(font, text) / 2;
}

static int font_draw_scaled_glyph(const sprite_t *glyph, uint16_t x, uint16_t y, uint8_t scale) {
    if (glyph == NULL || glyph->pixmap == NULL || scale == 0) return 1;

    for (uint16_t row = 0; row < glyph->height; row++) {
        for (uint16_t col = 0; col < glyph->width; col++) {
            const uint8_t *src = glyph->pixmap + (row * glyph->width + col) * 3;
            uint32_t color = src[0] | (src[1] << 8) | (src[2] << 16);

            if ((color & 0x00FFFFFF) != FONT_GLYPH_WHITE_888) continue;

            for (uint8_t dy = 0; dy < scale; dy++) {
                for (uint8_t dx = 0; dx < scale; dx++) {
                    if (ui_draw_pixel(x + col * scale + dx,
                                      y + row * scale + dy,
                                      color) != 0) {
                        return 1;
                    }
                }
            }
        }
    }

    return 0;
}

static int font_draw_scaled_glyph_color(const sprite_t *glyph, uint16_t x, uint16_t y, uint8_t scale, uint32_t target_color) {
    if (glyph == NULL || glyph->pixmap == NULL || scale == 0) return 1;

    for (uint16_t row = 0; row < glyph->height; row++) {
        for (uint16_t col = 0; col < glyph->width; col++) {
            const uint8_t *src = glyph->pixmap + (row * glyph->width + col) * 3;
            uint32_t color = src[0] | (src[1] << 8) | (src[2] << 16);

            if ((color & 0x00FFFFFF) != FONT_GLYPH_WHITE_888) continue;

            for (uint8_t dy = 0; dy < scale; dy++) {
                for (uint8_t dx = 0; dx < scale; dx++) {
                    if (ui_draw_pixel(x + col * scale + dx,
                                      y + row * scale + dy,
                                      target_color) != 0) {
                        return 1;
                    }
                }
            }
        }
    }

    return 0;
}

static int font_draw_half_glyph_color(const sprite_t *glyph, uint16_t x, uint16_t y, uint32_t target_color) {
    if (glyph == NULL || glyph->pixmap == NULL) return 1;

    for (uint16_t row = 0; row < glyph->height; row += 2) {
        for (uint16_t col = 0; col < glyph->width; col += 2) {
            bool draw_pixel = false;

            for (uint16_t src_row = row; src_row < row + 2 && src_row < glyph->height; src_row++) {
                for (uint16_t src_col = col; src_col < col + 2 && src_col < glyph->width; src_col++) {
                    const uint8_t *src = glyph->pixmap + (src_row * glyph->width + src_col) * 3;
                    uint32_t color = src[0] | (src[1] << 8) | (src[2] << 16);

                    if ((color & 0x00FFFFFF) == FONT_GLYPH_WHITE_888) {
                        draw_pixel = true;
                    }
                }
            }

            if (draw_pixel &&
                ui_draw_pixel(x + col / 2, y + row / 2, target_color) != 0) {
                return 1;
            }
        }
    }

    return 0;
}

int font_draw_scaled_text(const Font *font, const char *text, uint16_t x, uint16_t y, uint8_t scale) {
    if (font == NULL || text == NULL || font->loaded_chars < font->num_chars) return 1;

    uint16_t cur_x = x;

    for (const char *p = text; *p != '\0'; p++) {
        if (*p == ' ') {
            cur_x += font->char_width * scale;
            continue;
        }

        int idx = font_glyph_index(*p);
        if (idx < 0 || (uint32_t)idx >= font->num_chars) return 1;

        if (font_draw_scaled_glyph(&font->glyphs[idx], cur_x, y, scale) != 0) return 1;
        cur_x += font->char_width * scale;
    }

    return 0;
}

int font_draw_scaled_text_color(const Font *font, const char *text, uint16_t x, uint16_t y, uint8_t scale, uint32_t color) {
    if (font == NULL || text == NULL || font->loaded_chars < font->num_chars) return 1;

    uint16_t cur_x = x;

    for (const char *p = text; *p != '\0'; p++) {
        if (*p == ' ') {
            cur_x += font->char_width * scale;
            continue;
        }

        int idx = font_glyph_index(*p);
        if (idx < 0 || (uint32_t)idx >= font->num_chars) return 1;

        if (font_draw_scaled_glyph_color(&font->glyphs[idx], cur_x, y, scale, color) != 0) return 1;
        cur_x += font->char_width * scale;
    }

    return 0;
}

int font_draw_half_text_color(const Font *font, const char *text, uint16_t x, uint16_t y, uint32_t color) {
    if (font == NULL || text == NULL || font->loaded_chars < font->num_chars) return 1;

    uint16_t cur_x = x;
    uint16_t char_width = font->char_width / 2;

    for (const char *p = text; *p != '\0'; p++) {
        if (*p == ' ') {
            cur_x += char_width;
            continue;
        }

        int idx = font_glyph_index(*p);
        if (idx < 0 || (uint32_t)idx >= font->num_chars) return 1;

        if (font_draw_half_glyph_color(&font->glyphs[idx], cur_x, y, color) != 0) return 1;
        cur_x += char_width;
    }

    return 0;
}
