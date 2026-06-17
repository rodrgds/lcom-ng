#include "hud_font.h"

#include <ctype.h>
#include <stddef.h>
#include <string.h>

#include "hud_font_data.h"

static const char *supported_symbols = " 0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ:.-/?+><";

static xpm_map_t hud_font_glyph_xpms[HUD_FONT_GLYPH_COUNT] = {
  (xpm_map_t)hud_glyph_space,
  (xpm_map_t)hud_glyph_0,
  (xpm_map_t)hud_glyph_1,
  (xpm_map_t)hud_glyph_2,
  (xpm_map_t)hud_glyph_3,
  (xpm_map_t)hud_glyph_4,
  (xpm_map_t)hud_glyph_5,
  (xpm_map_t)hud_glyph_6,
  (xpm_map_t)hud_glyph_7,
  (xpm_map_t)hud_glyph_8,
  (xpm_map_t)hud_glyph_9,
  (xpm_map_t)hud_glyph_A,
  (xpm_map_t)hud_glyph_B,
  (xpm_map_t)hud_glyph_C,
  (xpm_map_t)hud_glyph_D,
  (xpm_map_t)hud_glyph_E,
  (xpm_map_t)hud_glyph_F,
  (xpm_map_t)hud_glyph_G,
  (xpm_map_t)hud_glyph_H,
  (xpm_map_t)hud_glyph_I,
  (xpm_map_t)hud_glyph_J,
  (xpm_map_t)hud_glyph_K,
  (xpm_map_t)hud_glyph_L,
  (xpm_map_t)hud_glyph_M,
  (xpm_map_t)hud_glyph_N,
  (xpm_map_t)hud_glyph_O,
  (xpm_map_t)hud_glyph_P,
  (xpm_map_t)hud_glyph_Q,
  (xpm_map_t)hud_glyph_R,
  (xpm_map_t)hud_glyph_S,
  (xpm_map_t)hud_glyph_T,
  (xpm_map_t)hud_glyph_U,
  (xpm_map_t)hud_glyph_V,
  (xpm_map_t)hud_glyph_W,
  (xpm_map_t)hud_glyph_X,
  (xpm_map_t)hud_glyph_Y,
  (xpm_map_t)hud_glyph_Z,
  (xpm_map_t)hud_glyph_colon,
  (xpm_map_t)hud_glyph_dot,
  (xpm_map_t)hud_glyph_hyphen,
  (xpm_map_t)hud_glyph_slash,
  (xpm_map_t)hud_glyph_question,
  (xpm_map_t)hud_glyph_plus,
  (xpm_map_t)hud_glyph_gt,
  (xpm_map_t)hud_glyph_lt,
};

// Linear scan over the small symbol set is pretty cheap (HUD_FONT_GLYPH_COUNT entries). If we had a larger set, we might want to switch to a "hash map" or something.
static int hud_font_symbol_index(char symbol)
{
  unsigned char normalized = (unsigned char)symbol;
  if (normalized >= 'a' && normalized <= 'z')
    normalized = (unsigned char)toupper(normalized);

  for (size_t index = 0; index < HUD_FONT_GLYPH_COUNT; index++)
  {
    if (supported_symbols[index] == (char)normalized)
      return (int)index;
  }

  return HUD_FONT_GLYPH_COUNT - 1;
}

int hud_font_load(hud_font_t *font)
{
  if (font == NULL)
    return 1;

  memset(font, 0, sizeof(*font));

  for (size_t index = 0; index < HUD_FONT_GLYPH_COUNT; index++)
  {
    if (vg_load_xpm_sprite(&font->glyphs[index], hud_font_glyph_xpms[index]) != 0)
    {
      hud_font_unload(font);
      return 1;
    }
  }

  font->loaded = true;
  return 0;
}

void hud_font_unload(hud_font_t *font)
{
  if (font == NULL)
    return;

  for (size_t index = 0; index < HUD_FONT_GLYPH_COUNT; index++)
    vg_destroy_sprite(&font->glyphs[index]);

  font->loaded = false;
}

uint16_t hud_font_line_height(uint16_t scale)
{
  if (scale == 0)
    return 0;

  return (uint16_t)((7U + 2U) * scale);
}

int hud_font_draw_text(const hud_font_t *font, const char *text, int16_t x, int16_t y, uint16_t scale)
{
  if (font == NULL || text == NULL || !font->loaded || scale == 0)
    return 1;

  int16_t cursor_x = x;
  int16_t cursor_y = y;
  int16_t origin_x = x;
  uint16_t line_height = hud_font_line_height(scale);

  for (const char *character = text; *character != '\0'; character++)
  {
    if (*character == '\n')
    {
      cursor_x = origin_x;
      cursor_y += (int16_t)line_height;
      continue;
    }

    int index = hud_font_symbol_index(*character);
    const vg_sprite_t *glyph = &font->glyphs[index];

    if (vg_draw_sprite_region_scaled_to_buffer(glyph, 0, 0, glyph->width, glyph->height,
                                              cursor_x, cursor_y, scale) != 0)
      return 1;

    cursor_x += (int16_t)((glyph->width + 1U) * scale);
  }

  return 0;
}

int hud_font_draw_text_colored(const hud_font_t *font, const char *text, int16_t x, int16_t y,
                              uint16_t scale, uint32_t color)
{
  if (font == NULL || text == NULL || !font->loaded || scale == 0)
    return 1;

  const uint32_t transparency = xpm_transparency_color(XPM_8_8_8_8);
  uint32_t opaque_color = color | 0xFF000000U;
  int16_t cursor_x = x;
  int16_t cursor_y = y;
  int16_t origin_x = x;
  uint16_t line_height = hud_font_line_height(scale);
  char *back_buffer = get_back_buffer();
  unsigned int h_res = get_h_res();
  unsigned int v_res = get_v_res();
  unsigned int bytes_per_pixel = get_bytes_per_pixel();
  // # of bytes per row
  size_t screen_stride = (size_t)h_res * bytes_per_pixel;

  if (back_buffer == NULL)
    return 1;

  for (const char *character = text; *character != '\0'; character++)
  {
    if (*character == '\n')
    {
      cursor_x = origin_x;
      cursor_y += (int16_t)line_height;
      continue;
    }

    int index = hud_font_symbol_index(*character);
    const vg_sprite_t *glyph = &font->glyphs[index];

    if (*character == ' ')
    {
      cursor_x += (int16_t)((glyph->width + 1U) * scale);
      continue;
    }

    for (uint16_t source_row = 0; source_row < glyph->height; source_row++)
    {
      for (uint16_t source_col = 0; source_col < glyph->width; source_col++)
      {
        const uint8_t *source_pixel = glyph->pixels +
                                      ((size_t)source_row * glyph->width + source_col) * glyph->bytes_per_pixel;
        uint32_t source_color = 0;
        memcpy(&source_color, source_pixel, sizeof(source_color));
        if (source_color == transparency)
          continue;

        for (uint16_t scale_row = 0; scale_row < scale; scale_row++)
        {
          int32_t screen_y = cursor_y + (int32_t)source_row * scale + scale_row;
          if (screen_y < 0 || screen_y >= (int32_t)v_res)
            continue;

          for (uint16_t scale_col = 0; scale_col < scale; scale_col++)
          {
            int32_t screen_x = cursor_x + (int32_t)source_col * scale + scale_col;
            if (screen_x < 0 || screen_x >= (int32_t)h_res)
              continue;

            uint8_t *destination = (uint8_t *)back_buffer + (size_t)screen_y * screen_stride +
                                  (size_t)screen_x * bytes_per_pixel;
            memcpy(destination, &opaque_color, bytes_per_pixel);
          }
        }
      }
    }

    cursor_x += (int16_t)((glyph->width + 1U) * scale);
  }

  return 0;
}
