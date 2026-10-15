#include <lcom/lcf.h>
#include "video.h"
#include "video_gr.h"
#include "vbe_macros.h"

static uint8_t *hidden_mem;
static uint8_t *dissolve_old_frame;
static void *vram;

static vbe_mode_info_t vmi;
static unsigned bytes_per_pixel;

uint16_t screen_width;
uint16_t screen_height;

int (video_init)(uint16_t mode) {
  if (vbe_get_mode_info(mode, &vmi) != 0) return 1;

  bytes_per_pixel = (vmi.BitsPerPixel + 7) / 8;
  screen_width = vmi.XResolution;
  screen_height = vmi.YResolution;

  vram = vg_init(mode);
  if (vram == NULL) return 1;

  size_t frame_size = (size_t)screen_width * screen_height * bytes_per_pixel;
  hidden_mem = malloc(frame_size);
  if (hidden_mem == NULL) return 1;
  dissolve_old_frame = malloc(frame_size);
  if (dissolve_old_frame == NULL) return 1;

  memset(hidden_mem, 0, frame_size);
  memset(dissolve_old_frame, 0, frame_size);
  return 0;
}

void (video_flip)() {
  memcpy(vram, hidden_mem, screen_width * screen_height * bytes_per_pixel);
}

void (video_clear)(uint32_t color) {
  size_t frame = (size_t)screen_width * screen_height * bytes_per_pixel;
  hidden_mem[0] = color & 0xFF;
  hidden_mem[1] = (color >> 8) & 0xFF;
  hidden_mem[2] = (color >> 16) & 0xFF;

  for (size_t done = bytes_per_pixel; done < frame; ) {
    size_t chunk = (done + done <= frame) ? done : frame - done;
    memcpy(hidden_mem + done, hidden_mem, chunk);
    done += chunk;
  }
}

int (video_draw_pixel)(uint16_t x, uint16_t y, uint32_t color) {
  if (x >= vmi.XResolution || y >= vmi.YResolution) return 0;

  uint8_t *pixel_ptr = hidden_mem + (y * vmi.XResolution + x) * bytes_per_pixel;
  memcpy(pixel_ptr, &color, bytes_per_pixel);

  return 0;
}

int (video_draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color) {
  if (x >= vmi.XResolution || y >= vmi.YResolution) return 0;

  uint8_t *row = hidden_mem + (y * vmi.XResolution + x) * bytes_per_pixel;
  for (uint16_t i = 0; i < len && x + i < vmi.XResolution; i++) memcpy(row + i * bytes_per_pixel, &color, bytes_per_pixel);

  return 0;
}

int (video_draw_rect)(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint32_t color) {
  for (int i = 0; i < h; i++) {
    if (video_draw_hline(x, y + i, w, color) != 0) return 1;
  }
  return 0;
}

void (video_draw_background)(const uint8_t *buf) {
  memcpy(hidden_mem, buf, screen_width * screen_height * bytes_per_pixel);
}

#define WAVE_BS 24
#define WAVE_COLS (1024 / WAVE_BS + 2)
#define WAVE_ROWS (768 / WAVE_BS  + 2)
#define WAVE_NOISE 200
#define WAVE_BAND 180

static int block_switch[WAVE_ROWS][WAVE_COLS];

void (video_start_dissolve)() {
  memcpy(dissolve_old_frame, hidden_mem, (size_t)screen_width * screen_height * bytes_per_pixel);

  int cols = screen_width  / WAVE_BS + 1;
  int rows = screen_height / WAVE_BS + 1;
  if (cols > WAVE_COLS) cols = WAVE_COLS;
  if (rows > WAVE_ROWS) rows = WAVE_ROWS;

  int max_dist = (cols - 1) + (rows - 1);

  for (int br = 0; br < rows; br++) {
    for (int bc = 0; bc < cols; bc++) {
      int base  = (bc + br) * (1000 - WAVE_BAND) / max_dist;
      int noise = (rand() % (2 * WAVE_NOISE + 1)) - WAVE_NOISE;
      int t = base + noise;

      if (t < 0) t = 0;
      if (t > 999) t = 999;

      block_switch[br][bc] = t;
    }
  }
}

void (video_draw_dissolve)(int tick, int max_ticks) {
  int bs = WAVE_BS;
  int cols = screen_width / bs + 1;
  int rows = screen_height / bs + 1;

  if (cols > WAVE_COLS) cols = WAVE_COLS;
  if (rows > WAVE_ROWS) rows = WAVE_ROWS;

  int progress = tick * 1000 / max_ticks;

  for (int br = 0; br < rows; br++) {
    for (int bc = 0; bc < cols; bc++) {
      int px = bc * bs;
      int py = br * bs;
      int pw = (px + bs > (int)screen_width) ? (int)screen_width - px : bs;
      int ph = (py + bs > (int)screen_height) ? (int)screen_height - py : bs;

      if (progress < block_switch[br][bc]) {
        for (int row = 0; row < ph; row++) {
          size_t off = ((size_t)(py + row) * screen_width + px) * bytes_per_pixel;
          memcpy(hidden_mem + off, dissolve_old_frame + off, (size_t)pw * bytes_per_pixel);
        }
        continue;
      }
      
      if (progress < block_switch[br][bc] + WAVE_BAND) {
        for (int row = 0; row < ph; row++) {
          uint8_t *line = hidden_mem + (size_t)(py + row) * screen_width * bytes_per_pixel;
          memset(line + (size_t)px * bytes_per_pixel, 0, (size_t)pw * bytes_per_pixel);
        }
      }
    }
  }
}

uint8_t *(video_cache_xpm)(xpm_map_t xpm) {
  xpm_image_t img;
  uint8_t *colors = xpm_load(xpm, XPM_8_8_8, &img);
  if (colors == NULL) return NULL;

  size_t buf_size = (size_t)screen_width * screen_height * 3;
  uint8_t *cache = malloc(buf_size);
  if (cache == NULL) { free(colors); return NULL; }
  memcpy(cache, colors, buf_size);
  free(colors);
  return cache;
}

int (video_draw_xpm_cached)(uint8_t *colors, xpm_image_t img, uint16_t x, uint16_t y) {
  uint8_t *px = colors;
  for (int i = 0; i < img.height; i++) {
    for (int j = 0; j < img.width; j++, px += 3) {
      uint32_t color = ((uint32_t)px[0] << 16) | ((uint32_t)px[1] << 8) | px[2];
      video_draw_pixel(x + j, y + i, color);
    }
  }
  return 0;
}
