#include <lcom/lcf.h>
#include <lcom/video_gr.h>

#include "../devices/video/video_gr.h"
#include "../xpm/font.xpm"
#include "text.h"

static xpm_image_t font_xpm; /**< @brief the xpm image of the font */

void(text_load)() {
  font_xpm = vg_load_xpm(xpm_font);
}

void(text_draw)(char *text, int16_t x, int16_t y, int16_t scale, int color, text_alignment_t alignment) {
  x -= alignment * strlen(text) * CHAR_WIDTH * scale / 2;

  do {
    char c = *text;
    int tx = c % FONT_WIDTH * CHAR_WIDTH;
    int ty = c / FONT_WIDTH * CHAR_HEIGHT;
    int ti = tx + ty * FONT_WIDTH * CHAR_WIDTH;

    for (int i = 0; i < CHAR_WIDTH; ++i)
      for (int j = 0; j < CHAR_HEIGHT; ++j)
        if (font_xpm.bytes[ti + i + j * FONT_WIDTH * CHAR_WIDTH])
          vg_draw_rectangle(x + i * scale, y + j * scale, scale, scale, color);

    x += CHAR_WIDTH * scale;
  } while (*(++text));
}

xpm_image_t(get_font_xpm)() {
  return font_xpm;
}
