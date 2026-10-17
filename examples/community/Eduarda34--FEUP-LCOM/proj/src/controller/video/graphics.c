#include <lcom/lcf.h>
#include "graphics.h"
#include <machine/int86.h> // For sys_int86 and struct reg86_t
#include <sys/mman.h>    // For vm_map_phys
#include <minix/syslib.h> // For sys_privctl
#include <stdio.h>       // For printf
#include <string.h>      // For memset
#include <stdint.h>      // For uint16_t, uint32_t
#include <stdlib.h>     // For malloc, free
#include "../keyboard/i8042.h"
#include "vbe.h"
#include "../../pixmap.h"
vbe_mode_info_t mode_info;
static uint8_t* back_buffer;
static uint8_t* video_mem;

int (graphic_mode)(uint16_t mode) {
    reg86_t r86;
    memset(&r86, 0, sizeof(r86));
    r86.intno = VBE_INTERRUPT;
    r86.ax = VBE_SET_MODE_FUNC;
    r86.bx = mode | VBE_SET_LINEAR_MODEL;
    if (sys_int86(&r86) != 0) {
        printf("graphicmode failed\n");
        return 1;
    }
    return 0;

}

int (frame_buffer)(uint16_t mode) {

    memset(&mode_info, 0, sizeof(mode_info));
    if (vbe_get_mode_info(mode, &mode_info) != 0) {
        printf("vbe_get_mode_info failed\n");
        return 1;
    }
    int r;
    struct minix_mem_range mr;
    unsigned int vram_base = (mode_info.BitsPerPixel + 7) / 8;
    unsigned int vram_size = mode_info.XResolution * mode_info.YResolution * vram_base;


    mr.mr_base = mode_info.PhysBasePtr;
    mr.mr_limit = mr.mr_base + vram_size;

    if ( OK != (r = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr))) {
        panic("sys_privctl (ADD_MEM) failed: %d\n", r);
    }
    // Map memory
    video_mem = vm_map_phys(SELF, (void *) mr.mr_base, vram_size);
    
    if (video_mem == MAP_FAILED) {
        panic("couldn’t map video memory");
    }

    back_buffer = malloc(vram_size);
    if (back_buffer == NULL) {
        printf("Failed to allocate memory for back buffer\n");
        return 1;
    }

    return 0;
}

void swap_buffer() {
     memcpy(video_mem, back_buffer,
           mode_info.XResolution * mode_info.YResolution * ((mode_info.BitsPerPixel + 7) / 8));
}

int (vg_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color) {
    for (unsigned i = 0; i < height; i++) {
        if (vg_draw_hline(x, y + i, width, color) != 0) {
            printf("vg_draw_hline failed\n");
            return 1;
        }
    }
    return 0;
}

int (vg_draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color) {
    for (unsigned i = 0; i < len; i++) {
        if (vg_draw_pixel(x + i, y, color) != 0) {
            printf("vg_draw_pixel failed\n");
            return 1;
        }
    }
    return 0;
}

int (vg_draw_pixel)(uint16_t x, uint16_t y, uint32_t color) {
    
    if (x >= mode_info.XResolution || y >= mode_info.YResolution) {
        printf("Coordinates out of bounds\n");
        return 1;
    }
    unsigned int bytes_per_pixel = (mode_info.BitsPerPixel + 7) / 8;
    unsigned int pixel_position = (y * mode_info.XResolution + x) * bytes_per_pixel;
    uint8_t *pixel = &back_buffer[pixel_position];

    if (bytes_per_pixel == 3) {
        pixel[0] = (color >> 16) & 0xFF;  // Red component
        pixel[1] = (color >> 8) & 0xFF;   // Green component
        pixel[2] = color & 0xFF;   
    } else if (bytes_per_pixel == 4) {
        *(uint32_t *)pixel = color;
    } else {
        // fallback for other bytes per pixel (like 1 or 2 bytes)
        memcpy(pixel, &color, bytes_per_pixel);
    }
    return 0;
}

int (vg_draw_xpm)(xpm_map_t xpm, uint16_t x, uint16_t y) {
    
    xpm_image_t img;
    uint8_t *map;

    map = xpm_load(xpm, XPM_8_8_8, &img);
    
    if (map == NULL) {
        printf("xpm_load failed\n");
        return 1;
    }

    for (unsigned i = 0; i < img.height; i++) {
        for (unsigned j = 0; j < img.width; j++) {
            unsigned int pos = (i * img.width + j) * 3; // 3 bytes per pixel (RGB)
            uint32_t color = (map[pos] << 16) | (map[pos + 1] << 8) | map[pos + 2]; // RGB
            if (vg_draw_pixel(x + j, y + i, color) != 0) {
                printf("vg_draw_pixel failed\n");
                free(map);
                return 1;
            }
        }
    }
    free(map);
    return 0;
}

void clear_screen() {
    unsigned int screen_size = mode_info.XResolution * mode_info.YResolution * 3;
    memset(back_buffer, 0, screen_size);

}

void (draw_score)(int score, uint16_t x, uint16_t y) {
    
    char str[12];
    sprintf(str, "%d", score);

    int digit_width = 10;

    for (int i = 0; str[i] != '\0'; i++) {
        int digit = str[i] - '0';
        if (digit < 0 || digit > 9) continue;

        xpm_map_t digit_xpm = get_digit_xpm(digit);
        vg_draw_xpm(digit_xpm, x + i * digit_width, y);
    }
}

