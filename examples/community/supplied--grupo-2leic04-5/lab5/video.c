#include "video.h"
#include <lcom/lcf.h>
#include <lcom/lcom.h>
#include <stdlib.h>
#include <string.h>
static lcom_vbe_mode_info_t mode_info;
static uint8_t *display;
static uint8_t *back;
int my_map_vram(uint16_t mode) { return lcom_vbe_get_mode_info(mode, &mode_info) == LCOM_OK ? 0 : 1; }
int my_vg_init(uint16_t mode) {
    display = vg_init(mode);
    return display ? 0 : 1;
}
int my_vg_enable_double_buffer(void) {
    if (!display || !mode_info.framebuffer_size) return 1;
    back = calloc(1, mode_info.framebuffer_size);
    return back ? 0 : 1;
}
int my_vg_draw_pixel(uint16_t x, uint16_t y, uint32_t color) {
    if (!back || x >= mode_info.width || y >= mode_info.height) return 1;
    uint8_t *p = back + (size_t)y * mode_info.pitch + (size_t)x * mode_info.bytes_per_pixel;
    for (unsigned b = 0; b < mode_info.bytes_per_pixel; ++b) p[b] = (uint8_t)(color >> (8 * b));
    return 0;
}
int my_vg_draw_hline(uint16_t x, uint16_t y, uint16_t len, uint32_t color) {
    if (y >= mode_info.height || x > mode_info.width || len > mode_info.width - x) return 1;
    for (unsigned col = 0; col < len; ++col) my_vg_draw_pixel((uint16_t)(x + col), y, color);
    return 0;
}
int my_vg_draw_rectangle(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color) {
    if (y > mode_info.height || height > mode_info.height - y) return 1;
    for (unsigned row = 0; row < height; ++row)
        if (my_vg_draw_hline(x, (uint16_t)(y + row), width, color)) return 1;
    return 0;
}
void my_vg_flip_buffer(void) {
    if (display && back) {
        memcpy(display, back, mode_info.framebuffer_size);
        lcom_vbe_present();
    }
}
void my_vg_free_double_buffer(void) { free(back); back = NULL; display = NULL; }
