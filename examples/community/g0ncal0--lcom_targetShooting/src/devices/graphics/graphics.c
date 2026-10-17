#include <lcom/lcf.h>
#include <lcom/lab5.h>

#include <stdint.h>
#include "graphics.h"

vbe_mode_info_t mode_info;
uint8_t* buffers[3];
int indexArrayBuffers = 0;
int vram_size;

int (set_graphics_mode)(uint16_t mode) {
    reg86_t reg;
    memset(&reg, 0, sizeof(reg));

    reg.intno = VIDEO_SERVICES;
    reg.ax = SET_VBE_MODE;
    reg.bx = mode | LINEAR_BIT;

    if(sys_int86(&reg) != OK) {
        printf("sys_int86 failed inside %s", __func__);
        return EXIT_FAILURE;
    }    

    return EXIT_SUCCESS;
}

int (set_frame_buffer)(uint16_t mode) {
    memset(&mode_info, 0, sizeof(mode_info));

    if (vbe_get_mode_info(mode, &mode_info) != OK) {
        printf("vbe_get_mode_info failed inside %s", __func__);
        return EXIT_FAILURE;
    }    

    vram_size = mode_info.XResolution * mode_info.YResolution * ((mode_info.BitsPerPixel + 7) / 8);

    struct minix_mem_range mr;
    mr.mr_base = mode_info.PhysBasePtr;
    mr.mr_limit = mr.mr_base + (vram_size * 3);

    if(sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr) != OK) {
        printf("sys_privctl failed inside %s", __func__);
        return EXIT_FAILURE;
    }

    for (int i = 0; i < 3; i++) {
        buffers[i] = vm_map_phys(SELF, (void *)(mr.mr_base + (vram_size * i)), vram_size);
        memset(buffers[i], 0, vram_size);

        if (buffers[i] == NULL) {
            printf("vm_map_phys failed inside %s", __func__);
            return EXIT_FAILURE;
        }
    }

    return EXIT_SUCCESS;
}

int (vg_draw_pixel)(uint16_t x, uint16_t y, uint32_t color) {
    if (x >= mode_info.XResolution) return OK;  
    if (y >= mode_info.YResolution) return OK;

    unsigned int bytes_per_pixel = (mode_info.BitsPerPixel + 7) / 8;
    uint8_t* pixel_pos = buffers[indexArrayBuffers] + ((y * mode_info.XResolution + x) * bytes_per_pixel);
    
    if (memcpy(pixel_pos, &color, bytes_per_pixel) == NULL) {
        printf("memcpy failed inside %s", __func__);
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

int (vg_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color) {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (vg_draw_pixel(x + j, y + i, color) != OK) {
                printf("vg_draw_pixel failed inside %s", __func__);
                return EXIT_FAILURE;
            }
        }
    }

    return EXIT_SUCCESS;
}

int (vg_page_flipping)() {
    reg86_t reg;
    memset(&reg, 0, sizeof(reg));

    reg.intno = VIDEO_SERVICES;
    reg.ax = SET_VBE_DISPLAY_START;
    reg.bl = VERTICAL_RETRACE;
    reg.cx = 0;
    reg.dx = indexArrayBuffers * mode_info.YResolution;;

    if(sys_int86(&reg) != 0) {
        printf("sys_int86 failed inside %s", __func__);
        return EXIT_FAILURE;
    }

    indexArrayBuffers = (indexArrayBuffers + 1) % 3;

    return EXIT_SUCCESS;
}

uint8_t* (get_actual_buffer)() {
    return buffers[indexArrayBuffers];
}

int (get_vram_size)() {
    return vram_size;
}

int (draw_background)(uint32_t* map) {
    memcpy(buffers[indexArrayBuffers], map, vram_size);

    return 0;
}
