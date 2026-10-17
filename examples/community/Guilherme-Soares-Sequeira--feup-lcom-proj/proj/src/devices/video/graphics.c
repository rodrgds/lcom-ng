#include <lcom/lcf.h>

#include "graphics.h"

static pixel_buffer backbuffer; /**< @brief back buffer of the vram */

void(load_backbuffer)() {
  backbuffer.buffer_start = vg_get_back_buffer();
  backbuffer.h_res = H_RES;
  backbuffer.v_res = V_RES;
  backbuffer.bytes_per_pixel = 1;
  backbuffer.buffer_size = backbuffer.h_res * backbuffer.v_res * backbuffer.bytes_per_pixel;
}

pixel_buffer const *(get_back_buffer) () {
  return &backbuffer;
}

int(buf_draw_pixel)(pixel_buffer const *const buf, position pos, uint8_t color) {
  if (pos.x >= buf->h_res || pos.y >= buf->v_res)
    return 0;

  int pixel_offset = pos.x + pos.y * buf->h_res;
  int byte_offset = pixel_offset * buf->bytes_per_pixel;

  uint8_t *pixel_start = (uint8_t *) buf->buffer_start + byte_offset;

  pixel_start[0] = color;

  return 0;
}

int(_buf_draw_line_low)(pixel_buffer const *const buf, uint16_t from_x, uint16_t from_y, uint16_t to_x, uint16_t to_y, uint8_t color) {
  int dx = to_x - from_x;
  int dy = to_y - from_y;
  int yi = 1;

  if (dy < 0) {
    yi = -1;
    dy = -dy;
  }

  int D = 2 * dy - dx;
  int y = from_y;

  for (int x = from_x; x <= to_x; x++) {
    if (buf_draw_pixel(buf, (position){x, y}, color)) {
      fprintf(stderr, "There was an error drawing a pixel at %s!\n", __func__);
      return 1;
    }

    if (D > 0) {
      y += yi;
      D += 2 * (dy - dx);
    }
    else
      D += 2 * dy;
  }

  return OK;
}

int(_buf_draw_line_high)(pixel_buffer const *const buf, uint16_t from_x, uint16_t from_y, uint16_t to_x, uint16_t to_y, uint8_t color) {
  int dx = to_x - from_x;
  int dy = to_y - from_y;
  int xi = 1;

  if (dx < 0) {
    xi = -1;
    dx = -dx;
  }

  int D = 2 * dx - dy;
  int x = from_x;

  for (int y = from_y; y <= to_y; y++) {
    if (buf_draw_pixel(buf, (position){x, y}, color)) {
      fprintf(stderr, "There was an error drawing a pixel at %s!\n", __func__);
      return 1;
    }

    if (D > 0) {
      x += xi;
      D += 2 * (dx - dy);
    }
    else
      D += 2 * dx;
  }

  return OK;
}

int(buf_draw_line)(pixel_buffer const *const buf, position from_pos, position to_pos, uint8_t color) {
  if (abs(to_pos.y - from_pos.y) < abs(to_pos.x - from_pos.x))
    if (from_pos.x > to_pos.x)
      return _buf_draw_line_low(buf, to_pos.x, to_pos.y, from_pos.x, from_pos.y, color);
    else
      return _buf_draw_line_low(buf, from_pos.x, from_pos.y, to_pos.x, to_pos.y, color);
  else if (from_pos.y > to_pos.y)
    return _buf_draw_line_high(buf, to_pos.x, to_pos.y, from_pos.x, from_pos.y, color);
  else
    return _buf_draw_line_high(buf, from_pos.x, from_pos.y, to_pos.x, to_pos.y, color);
}

int(buf_draw_rectangle)(pixel_buffer const *const buf, position pos, uint16_t width, uint16_t height, uint8_t color) {
  int i = (pos.x + pos.y * buf->h_res) * buf->bytes_per_pixel;
  int offset = buf->h_res * buf->bytes_per_pixel;

  width *= buf->bytes_per_pixel;

  for (int j = 0; j < height; ++j, i += offset)
    memset((uint8_t *) buf->buffer_start + i, color, width);

  return 0;
}

int(buf_draw_circle)(pixel_buffer const *const buf, position pos, uint16_t radius, uint8_t color) {
  const uint32_t squared_radius = pow(radius, 2);

  uint32_t x32b = pos.x, y32b = pos.y;

  for (uint32_t i = pos.x - radius; i <= (pos.x - radius) + 2 * radius; i++)
    for (uint32_t j = pos.y - radius; j <= (pos.y - radius) + 2 * radius; j++)
      if (((x32b - i) * (x32b - i) + (y32b - j) * (y32b - j)) <= squared_radius)
        if (buf_draw_pixel(buf, (position){i, j}, color)) {
          fprintf(stderr, "There was an error drawing a pixel at %s!\n", __func__);
          return 1;
        }

  return 0;
}

int(buf_draw_hline)(pixel_buffer const *const buf, position pos, uint16_t len, uint8_t color) {
  int byte_offset = (pos.y * buf->h_res + pos.x) * buf->bytes_per_pixel;
  len = pos.x + len >= buf->h_res ? buf->h_res - pos.x - 1 : len;

  memset((uint8_t *) buf->buffer_start + byte_offset, color, len * buf->bytes_per_pixel);
  return 0;
}

int(buf_draw_vline)(pixel_buffer const *const buf, position pos, uint16_t len, uint8_t color) {
  for (int i = 0; i < len && pos.y + i < buf->v_res; i++)
    if (buf_draw_pixel(buf, (position){pos.x, pos.y + i}, color))
      return 1;

  return 0;
}

int(buf_draw_xpm)(pixel_buffer const *const buf, const xpm_image_t xpm_info, position pos) {
  uint8_t *bytes = xpm_info.bytes;

  for (unsigned long int i = 0; i < (unsigned long) xpm_info.width * xpm_info.height; i++) {
    if (bytes[i] != COLOR_TRANSPARENT && buf_draw_pixel(
                                           buf,
                                           (position){pos.x + (i % xpm_info.width), pos.y + (i / xpm_info.width)},
                                           (uint8_t) * (bytes + i)))
      return 1;
  }

  return OK;
}

int(buf_draw_text)(pixel_buffer const *const buf, char *text, position pos, text_alignment_t alignment) {
  uint16_t scale = cursor_get_thickness();

  pos.x -= alignment * strlen(text) * CHAR_WIDTH * scale / 2;

  do {
    char c = *text;
    int tx = c % FONT_WIDTH * CHAR_WIDTH;
    int ty = c / FONT_WIDTH * CHAR_HEIGHT;
    int ti = tx + ty * FONT_WIDTH * CHAR_WIDTH;

    for (int i = 0; i < CHAR_WIDTH; ++i)
      for (int j = 0; j < CHAR_HEIGHT; ++j)
        if (get_font_xpm().bytes[ti + i + j * FONT_WIDTH * CHAR_WIDTH]) {
          if (pos.x + i * scale > buf->h_res || pos.y + j * scale > buf->v_res)
            continue;
          if (buf_draw_rectangle(buf, (position){pos.x + i * scale, pos.y + j * scale}, scale, scale, cursor_get_color())) {
            fprintf(stderr, "There was an error drawing a rectangle at %s!\n", __func__);
            return EXIT_FAILURE;
          }
        }

    pos.x += CHAR_WIDTH * scale;
  } while (*(++text));

  return OK;
}
