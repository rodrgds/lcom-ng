#include "renderer.h"

#include <stdbool.h>

#include <lcom/utils.h>

#include "graphics.h"

#define BYTES_PER_PIXEL_24BPP 3
#define BYTES_PER_PIXEL_32BPP 4

void renderer_clear(void) {
    uint8_t *buf  = video_get_draw_buffer();
    unsigned bpp  = get_bytes_per_pixel();
    unsigned w    = get_h_res();
    unsigned h    = get_v_res();
    unsigned size = w * h * bpp;

    memset(buf, 0, size);
}

static inline void set_pixel_buffer(uint8_t *buf, unsigned int idx, uint32_t color, unsigned bpp) {
    if (bpp == BYTES_PER_PIXEL_24BPP) {
        buf[idx]   = color & 0xFF;
        buf[idx+1] = (color >> 8) & 0xFF;
        buf[idx+2] = (color >> 16) & 0xFF;
    } else if (bpp == BYTES_PER_PIXEL_32BPP) {
        *(uint32_t*)(&buf[idx]) = color;
    } else {
        memcpy(&buf[idx], &color, bpp);
    }
}

static int renderer_draw_internal(int w, int h, const uint32_t *sprite_colors,
                                  int x, int y, const uint8_t *mask,
                                  int mask_w, int mask_h,
                                  bool mask_uses_screen_coords,
                                  uint32_t fill_color,
                                  RendererPixelColorFn color_fn,
                                  void *color_context) {
    if (w <= 0 || h <= 0) return 1;

    int screen_w = (int)get_h_res();
    int screen_h = (int)get_v_res();

    uint8_t *buf = video_get_draw_buffer();
    unsigned bpp = get_bytes_per_pixel();
    unsigned scanline = get_bytes_per_scanline();

    int start_row = (y < 0) ? -y : 0;
    int end_row = (y + h > screen_h) ? screen_h - y : h;

    int start_col = (x < 0) ? -x : 0;
    int end_col = (x + w > screen_w) ? screen_w - x : w;

    for (int col = start_col; col < end_col; col++) {
        int run_depth = 0;

        for (int row = start_row; row < end_row; row++) {
            int px = x + col;
            int py = y + row;

            if (mask != NULL) {
                int mask_col = mask_uses_screen_coords ? px : col;
                int mask_row = mask_uses_screen_coords ? py : row;

                if (mask_col < 0 || mask_col >= mask_w ||
                    mask_row < 0 || mask_row >= mask_h ||
                    mask[mask_col * mask_h + mask_row] == 0) {
                    run_depth = 0;
                    continue;
                }
            }

            uint32_t color = fill_color;
            if (color_fn != NULL) {
                color = color_fn(col, row, run_depth, color_context);
            } else if (sprite_colors != NULL) {
                color = sprite_colors[col + row * w];
                if (color == TRANSPARENT_COLOR) continue;
            }

            unsigned int idx = py * scanline + (px * bpp);
            set_pixel_buffer(buf, idx, color, bpp);
            run_depth++;
        }
    }

    return 0;
}

int renderer_draw_sprite(const Sprite *sprite, int x, int y) {
    if (sprite == NULL) return 1;
    return renderer_draw_internal(sprite_get_width(sprite), sprite_get_height(sprite),
                                  sprite_get_colors(sprite), x, y, NULL, 0, 0,
                                  false, 0, NULL, NULL);
}

int renderer_draw_terrain(const Sprite *sprite, int x, int y, const uint8_t *mask, int mask_w, int mask_h) {
    if (sprite == NULL || mask == NULL) return 1;
    return renderer_draw_internal(sprite_get_width(sprite), sprite_get_height(sprite),
                                  sprite_get_colors(sprite), x, y, mask, mask_w, mask_h,
                                  true, 0, NULL, NULL);
}

int renderer_draw_masked_pixels(int x, int y, const uint8_t *mask, int mask_w, int mask_h, RendererPixelColorFn color_fn, void *context) {
    if (mask == NULL || color_fn == NULL || mask_w <= 0 || mask_h <= 0) return 1;
    return renderer_draw_internal(mask_w, mask_h, NULL, x, y, mask, mask_w, mask_h,
                                  false, 0, color_fn, context);
}

int renderer_draw_rect(int x, int y, int w, int h, uint32_t color) {
    return renderer_draw_internal(w, h, NULL, x, y, NULL, 0, 0,
                                  false, color, NULL, NULL);
}

int renderer_flip(void) {
    return flip_page();
}
