#include "video.h"

#include <lcom/vbe.h>

#include <stdlib.h>
#include <string.h>

static uint8_t *video_mem;
static uint8_t *back_buffer;
static lcom_vbe_mode_info_t vmi;

uint16_t get_h_res(void) { return vmi.width; }
uint16_t get_v_res(void) { return vmi.height; }
uint8_t get_bits_per_pixel(void) { return vmi.bpp; }

void *vg_init(uint16_t mode) {
    if (lcom_vbe_get_mode_info(mode, &vmi) != LCOM_OK) return NULL;
    if (lcom_vbe_set_mode(mode) != LCOM_OK) return NULL;
    if (lcom_phys_map(vmi.framebuffer_phys, vmi.framebuffer_size, (void **)&video_mem) != LCOM_OK) {
        video_mem = NULL;
        return NULL;
    }
    back_buffer = (uint8_t *)calloc(vmi.framebuffer_size, 1);
    if (back_buffer == NULL) {
        lcom_phys_unmap(video_mem, vmi.framebuffer_size);
        video_mem = NULL;
        return NULL;
    }
    return video_mem;
}

int vg_exit(void) {
    if (video_mem != NULL) {
        lcom_phys_unmap(video_mem, vmi.framebuffer_size);
        video_mem = NULL;
    }
    return 0;
}

static void write_pixel(uint8_t *dst, uint32_t color) {
    dst[0] = (uint8_t)(color & 0xFFu);
    if (vmi.bytes_per_pixel > 1) dst[1] = (uint8_t)((color >> 8) & 0xFFu);
    if (vmi.bytes_per_pixel > 2) dst[2] = (uint8_t)((color >> 16) & 0xFFu);
    if (vmi.bytes_per_pixel > 3) dst[3] = (uint8_t)((color >> 24) & 0xFFu);
}

int vg_draw_pixel(uint16_t x, uint16_t y, uint32_t color) {
    if (back_buffer == NULL || x >= vmi.width || y >= vmi.height) return 1;
    uint8_t *px = back_buffer + (size_t)y * vmi.pitch + (size_t)x * vmi.bytes_per_pixel;
    write_pixel(px, color);
    return 0;
}

int vg_draw_hline(uint16_t x, uint16_t y, uint16_t len, uint32_t color) {
    if (back_buffer == NULL || y >= vmi.height || x >= vmi.width) return 1;
    if ((uint32_t)x + len > vmi.width) len = (uint16_t)(vmi.width - x);
    uint8_t pixel[4] = {0, 0, 0, 0};
    write_pixel(pixel, color);
    uint8_t *dst = back_buffer + (size_t)y * vmi.pitch + (size_t)x * vmi.bytes_per_pixel;
    for (uint16_t i = 0; i < len; i++) {
        memcpy(dst, pixel, vmi.bytes_per_pixel);
        dst += vmi.bytes_per_pixel;
    }
    return 0;
}

int vg_draw_rectangle(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color) {
    if (x >= vmi.width || y >= vmi.height) return 0;
    if ((uint32_t)x + width > vmi.width) width = (uint16_t)(vmi.width - x);
    if ((uint32_t)y + height > vmi.height) height = (uint16_t)(vmi.height - y);
    for (uint16_t row = 0; row < height; row++) {
        if (vg_draw_hline(x, (uint16_t)(y + row), width, color) != 0) return 1;
    }
    return 0;
}

int vg_draw_xpm(xpm_map_t xpm, enum xpm_image_type type, uint16_t x, uint16_t y) {
    xpm_image_t img;
    uint8_t *pixmap = xpm_load(xpm, type, &img);
    if (pixmap == NULL) return 1;
    int result = vg_draw_pixmap(pixmap, img.width, img.height, x, y);
    free(pixmap);
    return result;
}

void vg_flush(void) {
    if (video_mem == NULL || back_buffer == NULL) return;
    memcpy(video_mem, back_buffer, vmi.framebuffer_size);
    (void)lcom_vbe_present();
}

void vg_cleanup(void) {
    free(back_buffer);
    back_buffer = NULL;
}

void vg_copy_to_buffer(uint8_t *src) {
    if (back_buffer == NULL || src == NULL) return;
    memcpy(back_buffer, src, vmi.framebuffer_size);
}

int vg_draw_pixmap(uint8_t *pixmap, uint16_t width, uint16_t height, int x, int y) {
    if (back_buffer == NULL || pixmap == NULL) return 1;
    uint32_t transparent = xpm_transparency_color(XPM_8_8_8);
    for (uint16_t row = 0; row < height; row++) {
        int dy = y + row;
        if (dy < 0) continue;
        if (dy >= (int)vmi.height) break;
        for (uint16_t col = 0; col < width; col++) {
            int dx = x + col;
            if (dx < 0) continue;
            if (dx >= (int)vmi.width) break;
            const uint8_t *src = pixmap + ((size_t)row * width + col) * 3u;
            uint32_t color = (uint32_t)src[0] | ((uint32_t)src[1] << 8) | ((uint32_t)src[2] << 16);
            if (color == transparent) continue;
            uint8_t *dst = back_buffer + (size_t)dy * vmi.pitch + (size_t)dx * vmi.bytes_per_pixel;
            write_pixel(dst, color);
        }
    }
    return 0;
}
