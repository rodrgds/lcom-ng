#include <lcom/lcf.h>
#include <stdio.h>
#include <stdlib.h>
#include "gpu.h"


static vbe_mode_info_t info;
static void* buffer; // used for double buffering
static void* video_mem;
static uint16_t h_res, v_res;
static uint8_t bits_per_pixel;

int vg_set_mode(uint16_t mode) {
    reg86_t reg;
    memset(&reg, 0, sizeof(reg86_t));
    reg.ax = 0x4F02; // VBE call, function 02 -- set VBE mode
    reg.bx = 1 << 14 | mode; // set bit 14: linear framebuffer
    reg.intno = 0x10;
    if (sys_int86(&reg) != OK) {
        printf("set_vbe_mode: sys_int86() failed \n");
        return 1;
    }
    return 0;
}

void* (vg_init)(uint16_t mode) {
    vbe_get_mode_info(mode, &info);
    phys_bytes physical_address = info.PhysBasePtr;
    h_res = info.XResolution;
    v_res = info.YResolution;
    bits_per_pixel = info.BitsPerPixel;

    int err;
    struct minix_mem_range mr; /* physical memory range */
    unsigned int vram_base = physical_address; /* VRAM’s physical addresss */
    unsigned int vram_size = h_res * v_res * ((bits_per_pixel + 7) / 8); /* VRAM’s size, but you can use
    the frame-buffer size, instead */
    /* Allow memory mapping */
    mr.mr_base = (phys_bytes) vram_base;
    mr.mr_limit = mr.mr_base + vram_size;
    if (OK != (err = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr)))
        panic("sys_privctl (ADD_MEM) failed: %d\n", err);
    /* Map memory */
    video_mem = vm_map_phys(SELF, (void*)mr.mr_base, vram_size);
    if(video_mem == MAP_FAILED) {
        panic("couldn't map video memory");
        return NULL;
    }
    buffer = malloc(vram_size);
    memset(buffer, 0, h_res * v_res * ((bits_per_pixel + 7) / 8));
    if (!buffer) panic("couldn't allocate buffer");

    vg_set_mode(mode);

    return video_mem;
}

int vg_draw_pixel(uint16_t x, uint16_t y, uint32_t color) {
    // Round up for cases like 15 bits
    int bytes_per_pixel = (bits_per_pixel + 7) / 8;
    // Calculate the address of the pixel
    uint32_t* pixel = (uint32_t*) ((uint8_t*)buffer + bytes_per_pixel * (h_res * y + x)); 

    // Isolate each color component
    uint8_t r = (color >> 16) & 0xFF;
    uint8_t g = (color >> 8) & 0xFF;
    uint8_t b = color & 0xFF;

    // Calculate the masks for each color component
    // For example, for mode 0x11A (5:6:5), r_mask = 0xFA (11111000), because we want the 5 first bits of red
    uint16_t r_mask = (0xFF >> (8 - info.RedMaskSize)) << (8 - info.RedMaskSize);
    uint16_t g_mask = (0xFF >> (8 - info.GreenMaskSize)) << (8 - info.GreenMaskSize);
    uint16_t b_mask = (0xFF >> (8 - info.BlueMaskSize)) << (8 - info.BlueMaskSize);

    // Determine the shift amounts based on field positions
    // For example, for mode 0x11A (5:6:5), r_shift = 8, because we want red to be the first color to appear
    // in the result: 0x00FA << 8 = 0xFA00
    int r_shift = info.RedFieldPosition - (8 - info.RedMaskSize);
    int g_shift = info.GreenFieldPosition - (8 - info.GreenMaskSize);
    int b_shift = (8 - info.BlueFieldPosition) - info.BlueMaskSize;

    // Combine everything together
    *pixel = ((r & r_mask) << r_shift) |
             ((g & g_mask) << g_shift) |
             ((b & b_mask) >> b_shift);

    return 0;
}

int (vg_draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color) {
    for (int i = 0; i < len; i++) {
        vg_draw_pixel(x + i, y, color);
    }
    return 0;
}

int (vg_draw_vline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color) {
    for (int i = 0; i < len; i++) {
        vg_draw_pixel(x, y + i, color);
    }
    return 0;
}


int (vg_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color) {
    for (int i = 0; i < height; i++) {
        vg_draw_hline(x, y + i, width, color);
    }
    return 0;
}

int draw_xpm(xpm_map_t xpm, uint16_t x, uint16_t y) {
    xpm_image_t img;
    uint8_t* map = xpm_load(xpm, XPM_INDEXED, &img);
    printf("map = %p\n", map);

    for (int i = 0; i < img.height; i++) {
        for (int j = 0; j < img.width; j++) {
            vg_draw_pixel(x + j, y + i, map[i * img.width + j]);
        }
    }
    return 0;
}

void draw() {
    memcpy(video_mem, buffer, h_res * v_res * ((bits_per_pixel + 7) / 8));
}

int clear_screen() {
    // much faster than vg_draw_rectangle, prevents flickering
    memset(buffer, 0, h_res * v_res * ((bits_per_pixel + 7) / 8));
    return 0;
}

void freebuffer() {
    free(buffer);
}

uint16_t get_hres() {
    return h_res;
} 

uint16_t get_vres() {
    return h_res;
} 
