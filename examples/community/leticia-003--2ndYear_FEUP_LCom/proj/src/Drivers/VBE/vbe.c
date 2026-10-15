/**
 * @file vbe.c
 * @brief VBE (VESA BIOS Extensions) graphics driver implementation.
 *
 * This file provides functions for setting graphics modes, mapping VRAM,
 * drawing pixels, lines, rectangles, XPM images, double buffering, and drawing patterns.
 */

#include "vbe.h"
#include <math.h>

#define DRAW_PTR  ((uint8_t*)back_buffer)

void    *video_mem;            
uint8_t *back_buffer = NULL;   

phys_bytes vram_base;          
size_t vram_size;              
uint16_t h_res;               
uint16_t v_res;                
uint16_t bits_per_pixel;       
uint8_t bytes_per_pixel;       

/**
 * @brief Sets the graphics mode using VBE.
 *
 * Uses BIOS interrupt 0x10 to set the specified VBE mode.
 *
 * @param mode VBE mode to set.
 * @return 0 on success, 1 on failure.
 */
int set_graphics_mode(uint16_t mode) {
    reg86_t reg;
    memset(&reg, 0, sizeof(reg));

    reg.intno = 0x10;           
    reg.ah = 0x4F;             
    reg.al = 0x02;            
    reg.bx = BIT(14) | mode;  

    if (sys_int86(&reg) != OK) {
        printf("sys_int86() failed\n");
        return 1;
    }

    if (reg.ah != 0x00) { 
        printf("VBE set mode failed, AH = 0x%x\n", reg.ah);
        return 1;
    }
    return 0;
}

/**
 * @brief Sleeps for a given number of seconds.
 *
 * @param delay Number of seconds to sleep.
 */
void sleep_for_seconds(uint8_t delay) {
    tickdelay(micros_to_ticks(delay * 1000000));
}

/**
 * @brief Draws a pixel at (x, y) with the specified color.
 *
 * @param x X coordinate.
 * @param y Y coordinate.
 * @param color Color value.
 * @return 0 on success, 1 if out of bounds.
 */
int vg_draw_pixel(uint16_t x, uint16_t y, uint32_t color) {
    if (x >= h_res || y >= v_res) return 1;
    uint32_t bytes_per_pixel = (bits_per_pixel + 7) / 8;
    uint32_t location = (y * h_res + x) * bytes_per_pixel;
    uint8_t *video_ptr = DRAW_PTR;
    for (unsigned int i = 0; i < bytes_per_pixel; i++) {
        video_ptr[location + i] = (color >> (i * 8)) & 0xFF;
    }
    return 0;
}

/**
 * @brief Draws a horizontal line of pixels.
 *
 * @param x Starting X coordinate.
 * @param y Y coordinate.
 * @param len Length of the line in pixels.
 * @param color Color value.
 * @return 0 on success.
 */
int (vg_draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color) {
    for (uint16_t i = 0; i < len; i++) {
        vg_draw_pixel(x + i, y, color);
    }
    return 0;
}

/**
 * @brief Draws a filled rectangle.
 *
 * @param x Top-left X coordinate.
 * @param y Top-left Y coordinate.
 * @param width Rectangle width.
 * @param height Rectangle height.
 * @param color Color value.
 * @return 0 on success.
 */
int (vg_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color) {
    for (uint16_t i = 0; i < height; i++) {
        vg_draw_hline(x, y + i, width, color);
    }
    return 0;
}

/**
 * @brief Maps VRAM to the process's address space and initializes mode info.
 *
 * @param mode VBE mode to map.
 * @return 0 on success, 1 on failure.
 */
int map_vram(uint16_t mode) {
    struct minix_mem_range mr;
    vbe_mode_info_t mode_info;

    if (vbe_get_mode_info(mode, &mode_info)) {
        printf("Failed to get VBE mode info.\n");
        return 1;
    }

    h_res = mode_info.XResolution;
    v_res = mode_info.YResolution;
    bits_per_pixel = mode_info.BitsPerPixel;
    bytes_per_pixel = (bits_per_pixel + 7) / 8;

    vram_base = mode_info.PhysBasePtr;
    vram_size = h_res * v_res * ((bits_per_pixel + 7) / 8);

    mr.mr_base = vram_base;
    mr.mr_limit = vram_base + vram_size;

    if (sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr) != OK) {
        printf("sys_privctl failed.\n");
        return 1;
    }

    video_mem = vm_map_phys(SELF, (void *)vram_base, vram_size);
    if (video_mem == MAP_FAILED) {
        printf("vm_map_phys failed.\n");
        return 1;
    }

    return 0;
}

/**
 * @brief Draws an XPM image or sprite at the specified position.
 *
 * @param x Top-left X coordinate.
 * @param y Top-left Y coordinate.
 * @param width Image width.
 * @param height Image height.
 * @param bytes Pointer to image data.
 * @param square_shape True if the image is square and can be copied in blocks.
 * @return 0 on success.
 */
int vg_draw_xpm(int16_t x, int16_t y, uint16_t width, uint16_t height, uint8_t *bytes, bool square_shape) {
    if (x >= (int)h_res || y >= (int)v_res || x + width <= 0 || y + height <= 0) {
        printf("Sprite completely out of bounds: x=%d, y=%d, w=%u, h=%u, h_res=%u, v_res=%u\n", x, y, width, height, h_res, v_res);
        return 0;
    }

    const uint8_t bpp = bytes_per_pixel;
    const uint32_t row_bytes_src = width * 4;

    for (uint16_t i = 0; i < height; ++i) {
        int16_t sy = y + i;
        if (sy < 0 || sy >= (int)v_res) {
            continue;
        }

        uint8_t *dst = DRAW_PTR + (sy * h_res + (x < 0 ? 0 : x)) * bpp;
        const uint8_t *src = bytes + i * row_bytes_src;

        if (square_shape) {
            uint32_t visible = width;
            int16_t sx_offset = 0;
            if (x < 0) { sx_offset = -x; visible -= sx_offset; }
            if (x + width > h_res) visible = h_res - x - sx_offset;
            if (visible == 0) {
                continue;
            }

            const uint8_t *src_line = src + sx_offset * 4;
            memcpy(dst, src_line, visible * bpp);
            continue;
        }

        for (uint16_t j = 0; j < width; ++j) {
            int16_t sx = x + j;
            if (sx < 0 || sx >= (int)h_res) {
                continue;
            }

            const uint8_t *pix = src + j * 4;

            uint8_t b = pix[0];
            uint8_t g = pix[1];
            uint8_t r = pix[2];

            if (r == 170 && g == 170 && b == 170) continue;

            uint32_t colour =
                (bpp == 4 || bpp == 3)
                    ? (r << 16) | (g << 8) | b
                    : ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);

            memcpy(DRAW_PTR + (sy * h_res + sx) * bpp, &colour, bpp);
        }
    }

    return 0;
}

/**
 * @brief Clears the screen (back buffer) by filling it with zeros.
 */
void vg_clear_screen() {
    memset(back_buffer, 0, h_res * v_res * bytes_per_pixel);
}

/**
 * @brief Enables double buffering by allocating a back buffer.
 *
 * Call this after map_vram() succeeds.
 * @return 0 on success, 1 on failure.
 */
int vg_enable_double_buffering(void) {
    back_buffer = malloc(vram_size);
    if (!back_buffer) {
        printf("❌ could not allocate %zu-byte back buffer\n", vram_size);
        return 1;
    }
    memset(back_buffer, 0, vram_size);
    return 0;
}

/**
 * @brief Disables double buffering and frees the back buffer.
 */
void vg_disable_double_buffering(void) {
    if (back_buffer) { free(back_buffer); back_buffer = NULL; }
}

/**
 * @brief Swaps the back buffer to the front buffer (VRAM).
 */
void vg_swap_buffers(void) {
    memcpy(video_mem, back_buffer, vram_size);
}

/**
 * @brief Draws a pattern of rectangles with varying colors.
 *
 * @param no_rectangles Number of rectangles per row/column.
 * @param first Base color value.
 * @param step Color increment step.
 * @param mode_info Pointer to VBE mode info structure.
 */
void draw_pattern(uint8_t no_rectangles, uint32_t first, uint8_t step, const vbe_mode_info_t *mode_info) {
    uint16_t rectangle_width = h_res / no_rectangles;
    uint16_t rectangle_height = v_res / no_rectangles;

    uint32_t r_mask = (1 << mode_info->RedMaskSize) - 1;
    uint32_t g_mask = (1 << mode_info->GreenMaskSize) - 1;
    uint32_t b_mask = (1 << mode_info->BlueMaskSize) - 1;

    uint8_t r_base = (uint8_t)((((first >> mode_info->RedFieldPosition) & r_mask) * 255) / r_mask);
    uint8_t g_base = (uint8_t)((((first >> mode_info->GreenFieldPosition) & g_mask) * 255) / g_mask);
    uint8_t b_base = (uint8_t)((((first >> mode_info->BlueFieldPosition) & b_mask) * 255) / b_mask);

    for (uint8_t row = 0; row < no_rectangles; row++) {
        for (uint8_t col = 0; col < no_rectangles; col++) {
            uint8_t shade = (row * no_rectangles + col) * step;

            uint8_t r8 = (r_base + shade > 255) ? 255 : r_base + shade;
            uint8_t g8 = (g_base + shade > 255) ? 255 : g_base + shade;
            uint8_t b8 = (b_base + shade > 255) ? 255 : b_base + shade;

            uint32_t r = (r8 * r_mask) / 255;
            uint32_t g = (g8 * g_mask) / 255;
            uint32_t b = (b8 * b_mask) / 255;

            uint32_t color = (r << mode_info->RedFieldPosition) |
                             (g << mode_info->GreenFieldPosition) |
                             (b << mode_info->BlueFieldPosition);

            vg_draw_rectangle(col * rectangle_width, row * rectangle_height, rectangle_width, rectangle_height, color);
        }
    }
}



