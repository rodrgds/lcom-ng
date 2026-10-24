#include "background.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../graphics/graphics.h"
#include "../graphics/sprite.h"
#include "../assets/background.xpm"

#define STAR_COUNT 120
#define BRIGHT_STAR_COUNT 25
#define BIG_STAR_COUNT 8
#define BACKGROUND_SCROLL_DIVISOR 2

static Sprite background_sprite;
static uint8_t *background_cache = NULL;
static bool background_cache_ready = false;
static int background_scroll_counter = 0;

static uint32_t background_read_color(const Sprite *sprite, uint32_t index) {
  uint32_t color = 0;
  memcpy(&color, sprite->map + index * sprite->bytes_per_pixel, sprite->bytes_per_pixel);
  return color;
}

static void background_cache_draw_rect(int x, int y, int width, int height, uint32_t color) {
  if (background_cache == NULL || width <= 0 || height <= 0) return;
  int screen_width = graphics_get_width();
  int screen_height = graphics_get_height();

  if (x >= screen_width || y >= screen_height || x + width <= 0 || y + height <= 0) return;
  if (x < 0) { width += x; x = 0; }
  if (y < 0) { height += y; y = 0; }
  if (x + width > screen_width) { width = screen_width - x; }
  if (y + height > screen_height) { height = screen_height - y; }

  uint8_t bytes_per_pixel = graphics_get_bytes_per_pixel();
  uint16_t bytes_per_scanline = graphics_get_bytes_per_scanline();

  for (int row = y; row < y + height; row++) {
    uint8_t *buffer_row = background_cache + (size_t) row * bytes_per_scanline;
    if (bytes_per_pixel == 1) {
      memset(buffer_row + x, (uint8_t) color, (size_t) width);
      continue;
    }
    for (int col = x; col < x + width; col++) {
      memcpy(buffer_row + (size_t) col * bytes_per_pixel, &color, bytes_per_pixel);
    }
  }
}

static void background_cache_add_static_stars(void) {
  for (int i = 0; i < STAR_COUNT; i++) {
    int x = (i * 83 + (i % 7) * 19) % graphics_get_width();
    int y = (i * 47 + (i % 11) * 23) % graphics_get_height();
    uint32_t color = COLOR_WHITE;
    if (i % 5 == 0) color = COLOR_CYAN;
    else if (i % 7 == 0) color = COLOR_BLUE;
    background_cache_draw_rect(x, y, 1, 1, color);
  }

  for (int i = 0; i < BRIGHT_STAR_COUNT; i++) {
    int x = (i * 173 + 31) % graphics_get_width();
    int y = (i * 89 + 17) % graphics_get_height();
    background_cache_draw_rect(x, y, 2, 1, COLOR_WHITE);
  }
  
  for (int i = 0; i < BIG_STAR_COUNT; i++) {
    int x = (i * 211 + 60) % graphics_get_width();
    int y = (i * 137 + 41) % graphics_get_height();
    background_cache_draw_rect(x, y, 3, 1, COLOR_WHITE);
    background_cache_draw_rect(x + 1, y - 1, 1, 3, COLOR_WHITE);
  }
}

static int background_cache_build(void) {
  if (background_cache_ready) return 0;
  if (!background_sprite.loaded || background_sprite.map == NULL) return 1;

  size_t frame_size = graphics_get_frame_size();
  background_cache = malloc(frame_size);
  if (background_cache == NULL) return 1;

  memset(background_cache, 0, frame_size);

  int screen_width = graphics_get_width();
  int screen_height = graphics_get_height();
  int scale_x = (screen_width + background_sprite.width - 1) / background_sprite.width;
  int scale_y = (screen_height + background_sprite.height - 1) / background_sprite.height;
  int scale = scale_x > scale_y ? scale_x : scale_y;
  if (scale < 1) scale = 1;

  int scaled_width = background_sprite.width * scale;
  int scaled_height = background_sprite.height * scale;
  int offset_x = (screen_width - scaled_width) / 2;
  int offset_y = (screen_height - scaled_height) / 2;

  for (uint16_t row = 0; row < background_sprite.height; row++) {
    for (uint16_t col = 0; col < background_sprite.width; col++) {
      uint32_t index = (uint32_t) row * background_sprite.width + col;
      uint32_t color = background_read_color(&background_sprite, index);
      if (color == background_sprite.transparent_color) continue;
      background_cache_draw_rect(offset_x + col * scale, offset_y + row * scale, scale, scale, color);
    }
  }

  background_cache_add_static_stars();
  background_cache_ready = true;
  return 0;
}

static void draw_nebula_cloud(int center_x, int center_y, int scroll, uint32_t color_a, uint32_t color_b) {
  int screen_height = graphics_get_height();
  for (int i = 0; i < 90; i++) {
    int dx = ((i * 37) % 220) - 110;
    int dy = ((i * 53 + scroll / 2) % 140) - 70;
    int x = center_x + dx;
    int y = center_y + dy + scroll / 5;

    while (y >= screen_height) y -= screen_height;
    while (y < 0) y += screen_height;

    uint32_t color = color_a;
    if (i % 3 == 0) color = color_b;
    if (i % 11 == 0) color = COLOR_WHITE;

    graphics_draw_rectangle_clipped(x, y, 1 + (i % 3), 1, color);
    if (i % 9 == 0) graphics_draw_rectangle_clipped(x + 2, y + 1, 1, 1, color_b);
  }
}

void background_init(void) {
  if (sprite_load(&background_sprite, background_xpm) != 0) {
    printf("Error: could not load background sprite.\n");
  }
  background_cache_build();
}

void background_destroy(void) {
  sprite_destroy(&background_sprite);

  if (background_cache != NULL) {
    free(background_cache);
    background_cache = NULL;
  }

  background_cache_ready = false;
  background_scroll_counter = 0;
}

void background_draw(const Game *game) {
  int screen_width = graphics_get_width();
  int screen_height = graphics_get_height();

  if (background_cache_ready) {
    uint16_t scroll = 0;
    uint16_t source_y = 0;

    if (game->state != STATE_PAUSED) {
      background_scroll_counter++;
    }

    if (screen_height > 0) {
      scroll = (uint16_t) ((background_scroll_counter / BACKGROUND_SCROLL_DIVISOR) % screen_height);
      source_y = scroll == 0 ? 0 : (uint16_t) (screen_height - scroll);
    }

    graphics_copy_buffer_to_back_scrolled(background_cache, source_y);
    return;
  }

  graphics_clear_screen(COLOR_BLACK);
  int scroll = game->frame_counter;

  for (int i = 0; i < 170; i++) {
    int x = (i * 83 + (i % 7) * 19) % screen_width;
    int y = (i * 47 + scroll / 4) % screen_height;
    uint32_t color = COLOR_WHITE;
    if (i % 4 == 0) color = COLOR_BLUE;
    else if (i % 6 == 0) color = COLOR_CYAN;
    else if (i % 13 == 0) color = COLOR_MAGENTA;
    graphics_draw_rectangle_clipped(x, y, 1, 1, color);
  }

  for (int i = 0; i < 45; i++) {
    int x = (i * 173 + 31) % screen_width;
    int y = (i * 89 + scroll / 2) % screen_height;
    uint32_t color = COLOR_CYAN;
    if (i % 5 == 0) color = COLOR_WHITE;
    else if (i % 7 == 0) color = COLOR_MAGENTA;
    graphics_draw_rectangle_clipped(x, y, 2, 1, color);
  }

  for (int i = 0; i < 12; i++) {
    int x = (i * 211 + 60) % screen_width;
    int y = (i * 137 + scroll / 3) % screen_height;
    graphics_draw_rectangle_clipped(x, y, 3, 1, COLOR_WHITE);
    graphics_draw_rectangle_clipped(x + 1, y - 1, 1, 3, COLOR_WHITE);
  }

  draw_nebula_cloud(screen_width / 4, screen_height / 4, scroll, COLOR_BLUE, COLOR_MAGENTA);
  draw_nebula_cloud((screen_width * 3) / 4, screen_height / 2, scroll / 2, COLOR_CYAN, COLOR_BLUE);
  draw_nebula_cloud(screen_width / 2, (screen_height * 3) / 4, scroll / 3, COLOR_MAGENTA, COLOR_BLUE);
}
