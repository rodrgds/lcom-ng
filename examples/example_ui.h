#ifndef MACHINELAB_EXAMPLE_UI_H
#define MACHINELAB_EXAMPLE_UI_H

#include <lcom/lcom.h>

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "example_font.h"

typedef struct {
  uint8_t *fb;
  lcom_vbe_mode_info_t info;
} example_canvas_t;

static inline uint32_t example_rgb(uint8_t r, uint8_t g, uint8_t b) {
  return ((uint32_t)r << 16) | ((uint32_t)g << 8) | (uint32_t)b;
}

static inline void example_put_pixel(example_canvas_t *ctx, int x, int y, uint32_t color) {
  if (ctx == 0 || ctx->fb == 0) return;
  if (x < 0 || y < 0 || x >= (int)ctx->info.width || y >= (int)ctx->info.height) return;
  if (ctx->info.bytes_per_pixel < 3) return;
  uint8_t *px = ctx->fb + (uint32_t)y * ctx->info.pitch +
                (uint32_t)x * ctx->info.bytes_per_pixel;
  px[0] = (uint8_t)(color & 0xFFu);
  px[1] = (uint8_t)((color >> 8) & 0xFFu);
  px[2] = (uint8_t)((color >> 16) & 0xFFu);
  if (ctx->info.bytes_per_pixel >= 4) px[3] = 0xFF;
}

static inline void example_fill_rect(example_canvas_t *ctx, int x, int y, int w, int h,
                                     uint32_t color) {
  if (w <= 0 || h <= 0) return;
  int x0 = x < 0 ? 0 : x;
  int y0 = y < 0 ? 0 : y;
  int x1 = x + w;
  int y1 = y + h;
  if (x1 > (int)ctx->info.width) x1 = (int)ctx->info.width;
  if (y1 > (int)ctx->info.height) y1 = (int)ctx->info.height;
  for (int yy = y0; yy < y1; yy++) {
    for (int xx = x0; xx < x1; xx++) {
      example_put_pixel(ctx, xx, yy, color);
    }
  }
}

static inline void example_stroke_rect(example_canvas_t *ctx, int x, int y, int w, int h,
                                       int t, uint32_t color) {
  if (t <= 0) t = 1;
  example_fill_rect(ctx, x, y, w, t, color);
  example_fill_rect(ctx, x, y + h - t, w, t, color);
  example_fill_rect(ctx, x, y, t, h, color);
  example_fill_rect(ctx, x + w - t, y, t, h, color);
}

static inline const uint8_t *example_glyph_for(char c) {
  static const uint8_t unknown[7] = {0x0E, 0x11, 0x01, 0x06, 0x04, 0x00, 0x04};
  static const uint8_t punctuation[][7] = {
      {0x00, 0x04, 0x04, 0x00, 0x04, 0x04, 0x00}, /* : */
      {0x00, 0x00, 0x1F, 0x00, 0x1F, 0x00, 0x00}, /* = */
      {0x00, 0x00, 0x00, 0x1F, 0x00, 0x00, 0x00}, /* - */
      {0x00, 0x00, 0x00, 0x00, 0x00, 0x0C, 0x0C}, /* . */
      {0x04, 0x04, 0x04, 0x04, 0x04, 0x00, 0x04}, /* ! */
      {0x01, 0x02, 0x02, 0x04, 0x08, 0x08, 0x10}, /* / */
      {0x0E, 0x10, 0x10, 0x10, 0x10, 0x10, 0x0E}, /* [ */
      {0x0E, 0x01, 0x01, 0x01, 0x01, 0x01, 0x0E}, /* ] */
      {0x00, 0x00, 0x00, 0x00, 0x0C, 0x04, 0x08}, /* , */
      {0x00, 0x04, 0x04, 0x00, 0x04, 0x04, 0x08}, /* ; */
      {0x04, 0x04, 0x08, 0x00, 0x00, 0x00, 0x00}, /* ' */
      {0x10, 0x08, 0x08, 0x04, 0x02, 0x02, 0x01}, /* \ */
      {0x0A, 0x1F, 0x0A, 0x0A, 0x1F, 0x0A, 0x00}, /* # */
      {0x00, 0x04, 0x04, 0x1F, 0x04, 0x04, 0x00}, /* + */
      {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1F}, /* _ */
      {0x02, 0x04, 0x08, 0x10, 0x08, 0x04, 0x02}, /* < */
      {0x08, 0x04, 0x02, 0x01, 0x02, 0x04, 0x08}, /* > */
      {0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04}, /* | */
      {0x0A, 0x0A, 0x04, 0x00, 0x00, 0x00, 0x00}, /* " */
  };
  static const uint8_t glyphs[37][7] = {
      {0x0E, 0x11, 0x13, 0x15, 0x19, 0x11, 0x0E},
      {0x04, 0x0C, 0x04, 0x04, 0x04, 0x04, 0x0E},
      {0x0E, 0x11, 0x01, 0x02, 0x04, 0x08, 0x1F},
      {0x1F, 0x02, 0x04, 0x02, 0x01, 0x11, 0x0E},
      {0x02, 0x06, 0x0A, 0x12, 0x1F, 0x02, 0x02},
      {0x1F, 0x10, 0x1E, 0x01, 0x01, 0x11, 0x0E},
      {0x06, 0x08, 0x10, 0x1E, 0x11, 0x11, 0x0E},
      {0x1F, 0x01, 0x02, 0x04, 0x08, 0x08, 0x08},
      {0x0E, 0x11, 0x11, 0x0E, 0x11, 0x11, 0x0E},
      {0x0E, 0x11, 0x11, 0x0F, 0x01, 0x02, 0x0C},
      {0x0E, 0x11, 0x11, 0x1F, 0x11, 0x11, 0x11},
      {0x1E, 0x11, 0x11, 0x1E, 0x11, 0x11, 0x1E},
      {0x0E, 0x11, 0x10, 0x10, 0x10, 0x11, 0x0E},
      {0x1E, 0x11, 0x11, 0x11, 0x11, 0x11, 0x1E},
      {0x1F, 0x10, 0x10, 0x1E, 0x10, 0x10, 0x1F},
      {0x1F, 0x10, 0x10, 0x1E, 0x10, 0x10, 0x10},
      {0x0E, 0x11, 0x10, 0x17, 0x11, 0x11, 0x0F},
      {0x11, 0x11, 0x11, 0x1F, 0x11, 0x11, 0x11},
      {0x0E, 0x04, 0x04, 0x04, 0x04, 0x04, 0x0E},
      {0x01, 0x01, 0x01, 0x01, 0x11, 0x11, 0x0E},
      {0x11, 0x12, 0x14, 0x18, 0x14, 0x12, 0x11},
      {0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x1F},
      {0x11, 0x1B, 0x15, 0x15, 0x11, 0x11, 0x11},
      {0x11, 0x19, 0x15, 0x13, 0x11, 0x11, 0x11},
      {0x0E, 0x11, 0x11, 0x11, 0x11, 0x11, 0x0E},
      {0x1E, 0x11, 0x11, 0x1E, 0x10, 0x10, 0x10},
      {0x0E, 0x11, 0x11, 0x11, 0x15, 0x12, 0x0D},
      {0x1E, 0x11, 0x11, 0x1E, 0x14, 0x12, 0x11},
      {0x0F, 0x10, 0x10, 0x0E, 0x01, 0x01, 0x1E},
      {0x1F, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04},
      {0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x0E},
      {0x11, 0x11, 0x11, 0x11, 0x11, 0x0A, 0x04},
      {0x11, 0x11, 0x11, 0x15, 0x15, 0x15, 0x0A},
      {0x11, 0x11, 0x0A, 0x04, 0x0A, 0x11, 0x11},
      {0x11, 0x11, 0x0A, 0x04, 0x04, 0x04, 0x04},
      {0x1F, 0x01, 0x02, 0x04, 0x08, 0x10, 0x1F},
      {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}};

  if (c >= '0' && c <= '9') return glyphs[c - '0'];
  if (c >= 'A' && c <= 'Z') return glyphs[10 + c - 'A'];
  if (c >= 'a' && c <= 'z') return glyphs[10 + c - 'a'];
  if (c == ' ') return glyphs[36];
  if (c == ':') return punctuation[0];
  if (c == '=') return punctuation[1];
  if (c == '-') return punctuation[2];
  if (c == '.') return punctuation[3];
  if (c == '!') return punctuation[4];
  if (c == '/') return punctuation[5];
  if (c == '[') return punctuation[6];
  if (c == ']') return punctuation[7];
  if (c == ',') return punctuation[8];
  if (c == ';') return punctuation[9];
  if (c == '\'') return punctuation[10];
  if (c == '\\') return punctuation[11];
  if (c == '#') return punctuation[12];
  if (c == '+') return punctuation[13];
  if (c == '_') return punctuation[14];
  if (c == '<') return punctuation[15];
  if (c == '>') return punctuation[16];
  if (c == '|') return punctuation[17];
  if (c == '"') return punctuation[18];
  return unknown;
}

static inline uint32_t example_text_width(const char *text, uint8_t scale) {
  if (scale == 0) scale = 1;
  size_t len = 0;
  while (text[len] != 0) len++;
  if (len == 0) return 0;
  return (uint32_t)((len * 6u - 1u) * scale);
}

static inline void example_draw_text(example_canvas_t *ctx, int x, int y, const char *text,
                                     uint32_t color, uint8_t scale) {
  if (scale == 0) scale = 1;
  for (size_t i = 0; text[i] != 0; i++) {
    const uint8_t *glyph = example_glyph_for(text[i]);
    int gx = x + (int)i * 6 * (int)scale;
    for (int row = 0; row < 7; row++) {
      for (int col = 0; col < 5; col++) {
        if ((glyph[row] & (uint8_t)(1u << (4 - col))) == 0) continue;
        for (uint8_t sy = 0; sy < scale; sy++) {
          for (uint8_t sx = 0; sx < scale; sx++) {
            example_put_pixel(ctx, gx + col * scale + sx, y + row * scale + sy, color);
          }
        }
      }
    }
  }
}


/* Shared UI primitives. Positions and hit targets stay in framebuffer pixels. */
#define UI_BG 0xEEF0F2u
#define UI_PAPER 0xFFFFFFu
#define UI_INK 0x202C38u
#define UI_MUTED 0x586878u
#define UI_LINE 0xD3DCE2u
#define UI_ACCENT 0x246B5Du
#define UI_DARK 0x141D28u
#define UI_PANEL 0x202C3Bu
#define UI_LIGHT 0xEDF3F8u
#define UI_DIM 0xABBDCEu

static inline int example_label_width(const char *text, int scale) {
  int width = 0;
  for (; *text; text++) {
    unsigned ch = (unsigned char)*text;
    width += (scale == 2 ? example_bold_advance : example_ui_advance)[ch >= 32 && ch <= 126 ? ch - 32 : '?' - 32] * scale;
  }
  return width;
}

static inline void example_ink(example_canvas_t *ctx, int x, int y, uint32_t color, unsigned alpha) {
  if (!alpha || !ctx->fb || x < 0 || y < 0 || x >= ctx->info.width || y >= ctx->info.height || ctx->info.bytes_per_pixel < 3) return;
  uint8_t *px = ctx->fb + (uint32_t)y * ctx->info.pitch + (uint32_t)x * ctx->info.bytes_per_pixel;
  for (unsigned channel = 0; channel < 3; channel++)
    px[channel] = (uint8_t)((((color >> (channel * 8)) & 255) * alpha + px[channel] * (15 - alpha) + 7) / 15);
  if (ctx->info.bytes_per_pixel >= 4) px[3] = 255;
}

static inline void example_label(example_canvas_t *ctx, int x, int y,
                                  const char *text, uint32_t color, int scale) {
  for (; *text; text++) {
    unsigned ch = (unsigned char)*text;
    unsigned index = ch >= 32 && ch <= 126 ? ch - 32 : '?' - 32;
    if (scale == 2) {
      for (int row = 0; row < 36; row++) for (int col = 0; col < 32; col++) {
        unsigned packed = example_heading_pixels[index][row][col / 2];
        unsigned alpha = (col & 1) ? packed & 15u : packed >> 4;
        if (alpha) example_ink(ctx, x + col, y + row, color, alpha);
      }
      x += example_bold_advance[index] * 2;
      continue;
    }
    for (int row = 0; row < 18; row++) {
      uint64_t bits = example_ui_rows[index][row];
      for (int col = 0; col < 16; col++) {
        unsigned alpha = (unsigned)((bits >> ((15 - col) * 4)) & 15);
        if (!alpha) continue;
        for (int sy = 0; sy < scale; sy++) for (int sx = 0; sx < scale; sx++)
          example_ink(ctx, x + col * scale + sx, y + row * scale + sy, color, alpha);
      }
    }
    x += (scale == 2 ? example_bold_advance[index] : example_ui_advance[index]) * scale;
  }
}

static inline void example_mono(example_canvas_t *ctx, int x, int y,
                                 const char *text, uint32_t color) {
  for (; *text; text++, x += 9) {
    unsigned ch = (unsigned char)*text;
    unsigned index = ch >= 32 && ch <= 126 ? ch - 32 : '?' - 32;
    for (int row = 0; row < 18; row++) for (int col = 0; col < 9; col++)
      example_ink(ctx, x + col, y + row, color,
        (unsigned)((example_mono_rows[index][row] >> ((8 - col) * 4)) & 15));
  }
}

static inline void example_round_rect(example_canvas_t *ctx, int x, int y,
                                       int w, int h, int radius, uint32_t color) {
  for (int row = 0; row < h; row++) {
    int dy = row < radius ? radius - row - 1 : row >= h - radius ? row - (h - radius) : 0;
    int inset = 0;
    while (inset < radius && (radius - inset) * (radius - inset) + dy * dy > radius * radius) inset++;
    example_fill_rect(ctx, x + inset, y + row, w - 2 * inset, 1, color);
  }
}

static inline void example_button(example_canvas_t *ctx, int x, int y, int w, int h,
                                   const char *label, bool active, bool hover, bool enabled) {
  uint32_t fill = !enabled ? 0xDFE5E9 : active ? UI_ACCENT : hover ? 0xD2E5DF : 0xE5EBEF;
  example_round_rect(ctx, x, y, w, h, 6, fill);
  example_label(ctx, x + (w - example_label_width(label, 1)) / 2,
                y + (h - 18) / 2, label, !enabled ? UI_MUTED : active ? UI_PAPER : UI_INK, 1);
}

static inline void example_heading(example_canvas_t *ctx, const char *title,
                                    const char *subtitle, uint32_t accent, bool dark) {
  example_fill_rect(ctx, 0, 0, 800, 76, dark ? UI_DARK : UI_PAPER);
  example_fill_rect(ctx, 24, 74, 752, 1, dark ? 0x354456 : UI_LINE);
  example_label(ctx, 24, 10, title, dark ? UI_LIGHT : UI_INK, 2);
  example_label(ctx, 26, 46, subtitle, dark ? UI_DIM : UI_MUTED, 1);
  example_round_rect(ctx, 752, 25, 16, 16, 4, accent);
}

static inline void example_dark_button(example_canvas_t *ctx, int x, int y, int w, int h,
                                        const char *label, bool active, bool hover, bool enabled) {
  uint32_t fill = !enabled ? 0x263340 : active ? 0x286D60 : hover ? 0x394E64 : UI_PANEL;
  example_round_rect(ctx, x, y, w, h, 6, fill);
  example_label(ctx, x + (w - example_label_width(label, 1)) / 2, y + (h - 18) / 2,
                label, !enabled ? 0x8E9EAE : UI_LIGHT, 1);
}

static inline int example_number_width(const char *text) {
  int width = 0;
  while (*text++) width += 58;
  return width;
}

static inline void example_number(example_canvas_t *ctx, int x, int y, const char *text,
                                   uint32_t color) {
  for (; *text; text++, x += 58) {
    unsigned index = *text >= '0' && *text <= '9' ? (unsigned)(*text - '0')
                   : *text == ':' ? 10 : *text == '.' ? 11 : *text == '-' ? 12 : 13;
    for (int row = 0; row < 80; row++) for (int col = 0; col < 58; col++) {
      unsigned packed = example_number_pixels[index][row][col / 2];
      unsigned alpha = col % 2 ? packed & 15 : packed >> 4;
      if (alpha) example_ink(ctx, x + col, y + row, color, alpha);
    }
  }
}

/* Preserve left/right modifiers independently, including PS/2 E0 Command keys. */
typedef struct {
  bool extended;
  uint8_t ctrl;
  uint8_t command;
  uint8_t shift;
} example_modifiers_t;

static inline bool example_modifier_byte(example_modifiers_t *keys, uint8_t data) {
  if (data == 0xE0) { keys->extended = true; return false; }
  uint8_t code = data & 0x7F;
  uint8_t *state = 0;
  uint8_t bit = 1;
  if (code == 0x1D) { state = &keys->ctrl; bit = keys->extended ? 2 : 1; }
  else if (keys->extended && (code == 0x5B || code == 0x5C)) {
    state = &keys->command; bit = code == 0x5B ? 1 : 2;
  } else if (!keys->extended && (code == 0x2A || code == 0x36)) {
    state = &keys->shift; bit = code == 0x2A ? 1 : 2;
  }
  keys->extended = false;
  if (!state) return false;
  if (data & 0x80) *state &= (uint8_t)~bit;
  else *state |= bit;
  return true;
}

static inline bool example_primary(const example_modifiers_t *keys) {
  return keys->ctrl != 0 || keys->command != 0;
}

#endif
