#include <lcom/lcf.h>
#include <lcom/utils.h>

#include "graphics.h"

static char *video_mem;
static vbe_mode_info_t vmi;
static unsigned bytes_per_pixel;

static uint8_t *back_buffer;
static unsigned int vram_size;

static uint8_t* current_background = NULL;

int set_video_mode(uint16_t mode) {
    reg86_t reg;
    memset(&reg, 0, sizeof(reg));

    reg.intno = 0x10;
    reg.ah = 0x4F;
    reg.al = 0x02;
    reg.bx = mode | 0x4000;

    if (sys_int86(&reg) != OK) {
        printf("sys_int86() failed \n");
        return 1;
    }

    return 0;
}

int vram_start(uint16_t mode) {
    struct minix_mem_range mr;

    if (vbe_get_mode_info(mode, &vmi) != 0) {
        printf("vbe_mode_info failed\n");
        return 1;
    }

    vram_size = vmi.YResolution * vmi.BytesPerScanLine;
    bytes_per_pixel = (vmi.BitsPerPixel + 7) / 8;
    //FOR SPRITES
    back_buffer = malloc(vram_size);
    if (!back_buffer)
        return 1;

    mr.mr_base = (phys_bytes)vmi.PhysBasePtr;
    mr.mr_limit = mr.mr_base + vram_size;

    if (sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr) != OK) return 1;
    video_mem = vm_map_phys(SELF, (void *)mr.mr_base, vram_size);

    if (video_mem == MAP_FAILED) return 1;

    return 0;
}

int(vg_draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color) {
    if (y >= vmi.YResolution) return 1;
    if (x >= vmi.XResolution) return 0;
    if (x + len > vmi.XResolution) len = vmi.XResolution - x;

    unsigned int offset = y * vmi.BytesPerScanLine + x * bytes_per_pixel;
    uint8_t *dest = back_buffer + offset;

    if (bytes_per_pixel == 1) {
        memset(dest, color, len);
    } else if (bytes_per_pixel == 2) {
        uint16_t val16 = (uint16_t)color;
        uint16_t *dest16 = (uint16_t *)dest;
        for (uint16_t i = 0; i < len; i++) {
            dest16[i] = val16;
        }
    } else if (bytes_per_pixel == 3) {
        for (uint16_t i = 0; i < len; i++) {
            dest[0] = color & 0xFF;
            dest[1] = (color >> 8) & 0xFF;
            dest[2] = (color >> 16) & 0xFF;
            dest += 3;
        }
    } else if (bytes_per_pixel == 4) {
        uint32_t val32 = color;
        uint32_t *dest32 = (uint32_t *)dest;
        for (uint16_t i = 0; i < len; i++) {
            dest32[i] = val32;
        }
    }
    return 0;
}

int vg_draw_pixel(uint16_t x, uint16_t y, uint32_t color) {
    if (x >= vmi.XResolution || y >= vmi.YResolution) return 1;
    unsigned int offset = y * vmi.BytesPerScanLine + x * bytes_per_pixel;
    uint8_t *dest = back_buffer + offset;
    if (bytes_per_pixel == 4) {
        *(uint32_t *)dest = color;
    } else if (bytes_per_pixel == 2) {
        *(uint16_t *)dest = (uint16_t)color;
    } else if (bytes_per_pixel == 1) {
        *dest = (uint8_t)color;
    } else {
        memcpy(dest, &color, bytes_per_pixel);
    }
    return 0;
}

int(vg_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color) {
    for (uint16_t row = y; row < y + height; row++) vg_draw_hline(x, row, width, color);
    
    return 0;
}

unsigned int get_bytes_per_pixel(){
    return bytes_per_pixel;
}

void clear_buffer() {
    memset(back_buffer, 0, vram_size);
}

void swap_buffer() {
    memcpy(video_mem, back_buffer, vram_size);
}

int draw_xpm(xpm_image_t img, uint8_t *colors, int x, int y) {
    int v_res = get_v_res();
    int h_res = get_h_res();

    for (int i = 0; i < img.height; i++) {
        int target_y = y + i;
        if (target_y < 0 || target_y >= v_res) continue;

        for (int j = 0; j < img.width; j++) {
            int target_x = x + j;
            if (target_x < 0 || target_x >= h_res) continue;

            int idx = (i * img.width + j) * 4;

            uint8_t alpha = colors[idx + 3];
            if (alpha != 0) continue; 

            uint32_t color = ((uint32_t) colors[idx + 2] << 16) | ((uint32_t) colors[idx + 1] << 8) | (uint32_t) colors[idx];
            vg_draw_pixel(target_x, target_y, color);
        }
    }
    return 0;
}

int draw_xpm_scaled(xpm_image_t img, uint8_t *colors, int x, int y, int scale) {
    if (scale < 1) scale = 1;
    int v_res = get_v_res();
    int h_res = get_h_res();
    for (int i = 0; i<img.height; i++) {
        for (int j = 0; j<img.width; j++) {
            int idx = (i * img.width + j) * 4;
            uint8_t alpha = colors[idx + 3];
            if (alpha != 0) continue;
            uint32_t color = ((uint32_t) colors[idx + 2] << 16) | ((uint32_t) colors[idx + 1] << 8) | (uint32_t) colors[idx];
            for (int dy = 0; dy <scale; dy++) {
                int target_y = y + i * scale + dy;
                if (target_y < 0 || target_y >= v_res) continue;
                for (int dx = 0; dx < scale; dx++) {
                    int target_x = x + j * scale + dx;
                    if (target_x < 0 || target_x >= h_res) continue;
                    vg_draw_pixel(target_x, target_y, color);
                }
            }
        }
    }
    return 0;
}

uint16_t get_h_res()
{
    return vmi.XResolution;
}

uint16_t get_v_res()
{
    return vmi.YResolution;
}

void (set_static_background)(uint8_t* background_buffer) {
    current_background = background_buffer;
}

void (provide_static_background)() {
    if (current_background == NULL) return;

    memcpy(back_buffer, current_background, vram_size);
}

void (delete_static_background)() {
    if (current_background == NULL) return;

    free(current_background);
    current_background = NULL;
}

uint8_t* (get_back_buffer)() {
    return back_buffer;
}

unsigned int (get_vram_size)() {
    return vram_size;
}


