#include <lcom/lcf.h>
#include "video.h"

vbe_mode_info_t info;
static uint8_t *buffer;

int (video_set_mode)(uint16_t mode) {
    reg86_t r;
    memset(&r, 0, sizeof(r));
    r.ax = 0x4F02; 
    r.bx = mode | BIT(14); 
    r.intno = 0x10;
    if (sys_int86(&r) != OK) {
        printf("video_set_mode: sys_int86() failed\n");
        return 1;
    }
    return 0;
}

int (video_map_memory)(uint16_t mode) {
    memset(&info, 0, sizeof(info));

    if (vbe_get_mode_info(mode, &info) != OK) {
        printf("video_map_memory: vbe_get_mode_info() failed\n");
        return 1;
    }

    struct minix_mem_range address;
    unsigned int vram_base;
    unsigned int vram_size;

    vram_base = info.PhysBasePtr;
    vram_size = info.XResolution * info.YResolution * (info.BitsPerPixel + 7 / 8);

    address.mr_base = (phys_bytes) vram_base;
    address.mr_limit = address.mr_base + vram_size;

    if (sys_privctl(SELF, SYS_PRIV_ADD_MEM, &address) != OK) {
        printf("video_map_memory: sys_privctl() failed\n");
        return 1;
    }

    buffer = vm_map_phys(SELF, (void *) vram_base, vram_size);
    if (buffer == MAP_FAILED) {
        printf("video_map_memory: vm_map_phys() failed\n");
        return 1;
    }

    return 0;
}

int (vg_draw_pixel)(uint16_t x, uint16_t y, uint32_t color) {
    if (x >= info.XResolution || y >= info.YResolution || x < 0 || y < 0) {
        printf("vg_draw_pixel: coordinates out of bounds\n");
        return 1;
    }

    unsigned int bytes_per_pixel = (info.BitsPerPixel + 7) / 8;
    unsigned int offset = (y * info.XResolution + x) * bytes_per_pixel;

    memcpy(&buffer[offset], &color, bytes_per_pixel);

    return 0;
}

int (vg_draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color) {
    for (uint16_t i = 0; i < len; i++) {
        if(vg_draw_pixel(x + i, y, color)) return 1; 
    }
    return 0;
}

int (vg_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color) {
    for (uint16_t i = 0; i < height; i++) {
        if (vg_draw_hline(x, y + i, width, color)) {
            vg_exit();
            return 1;
        }
    }
    return 0;
}

int (vg_draw_xpm)(xpm_map_t xpm, uint16_t x, uint16_t y) {

    xpm_image_t img;
    uint8_t *map = xpm_load(xpm, XPM_INDEXED, &img);

    if (map == NULL) {
        printf("vg_draw_xpm: xpm_load() failed\n");
        return 1;
    }

    for (uint16_t i = 0; i < img.height; i++) {
        for (uint16_t j = 0; j < img.width; j++) {
            if (vg_draw_pixel(x + j, y + i, *map)) return 1;
            map++;
        }
    }
    return 0;
}

uint32_t (indexed_color)(uint16_t row, uint16_t col, uint8_t no_rectangles, uint32_t first,  uint8_t step){
    return (first + (row * no_rectangles + col) * step) % (1 << info.BitsPerPixel);
}

uint32_t (direct_color)(uint16_t row, uint16_t col, uint8_t no_rectangles, uint32_t first,  uint8_t step){
    uint32_t R_first = (first >> info.RedFieldPosition) & ((1 << info.RedMaskSize) - 1);
    uint32_t G_first = (first >> info.GreenFieldPosition) & ((1 << info.GreenMaskSize) - 1);
    uint32_t B_first = (first >> info.BlueFieldPosition) & ((1 << info.BlueMaskSize) - 1);

    uint32_t R = (R_first + col * step) % (1 << info.RedMaskSize);
    uint32_t G = (G_first + row * step) % (1 << info.GreenMaskSize);
    uint32_t B = (B_first + (row + col) * step) % (1 << info.BlueMaskSize);

    return (R << info.RedFieldPosition) | (G << info.GreenFieldPosition) | (B << info.BlueFieldPosition);
}
