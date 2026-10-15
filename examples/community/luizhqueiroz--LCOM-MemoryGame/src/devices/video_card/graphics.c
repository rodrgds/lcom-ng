#include "graphics.h"

int (set_graphic_mode)(uint16_t mode) {
    reg86_t reg86;

    memset(&reg86, 0, sizeof(reg86));

    reg86.intno = INT_INST;
    reg86.ax = SET_VBE_MODE; 
    reg86.bx = LINEAR_MODE | mode;
    

    if (sys_int86(&reg86) != OK) {
        printf("sys_int86() failed \n");
        return 1;
    }

    return 0;
}

int (set_text_mode)() {
    reg86_t reg86;

    memset(&reg86, 0, sizeof(reg86));

    reg86.intno = INT_INST;
    reg86.ah = SET_VIDEO_MODE;
    reg86.al = SET_TEXT_MODE; 

    if (sys_int86(&reg86) != OK) {
        printf("sys_int86() failed \n");
        return 1;
    }

    return 0;
}


int (map_vram)(uint16_t mode) {
    struct minix_mem_range mr;

    unsigned int frame_size;

    int r;

    memset(&mode_info, 0, sizeof(mode_info));

    if (vbe_get_mode_info(mode, &mode_info)) {
        return 1;
    }

    h_res = mode_info.XResolution;
    v_res = mode_info.YResolution;
    bytes_per_pixel = (mode_info.BitsPerPixel + 7) / 8;
    frame_size = h_res * v_res * bytes_per_pixel;

    mr.mr_base = mode_info.PhysBasePtr;
    mr.mr_limit = mr.mr_base + frame_size;

    if ((r = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr)) != OK) {
        panic("sys_privctl (ADD_MEM) failed: %d\n", r);
        return 1;
    }

    frame_adress = vm_map_phys(SELF, (void *) mr.mr_base, frame_size);
    if(frame_adress == MAP_FAILED) {
        panic("couldn't map video memory");
        return 1;
    }
    
    return 0;
}

int (vg_draw_pixel)(uint16_t x, uint16_t y, uint32_t color) {
    if (x >= h_res || y >= v_res || x < 0 || y < 0 || color == TRANSPARENCY) {
        return 0;
    }

    unsigned int index = (y*h_res + x)*bytes_per_pixel;

    if (memcpy(&back_buffer[index], &color, bytes_per_pixel) == NULL) {
        return 1;
    }

    return 0;
}

int (vg_draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color) {
    for (unsigned i = 0; i < len; i++ ) {
        if (vg_draw_pixel(x + i, y, color)) {
            return 1;
        }
    }

    return 0;
}

int (vg_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color) {
    for (unsigned i = 0; i < height; i++) {
        if (vg_draw_hline(x, y + i, width, color)) {
            vg_exit();
            return 1;
        }
    }

    return 0;
}

int (display_xpm)(xpm_map_t xpm, uint16_t x, uint16_t y) {

    xpm_image_t img;

    uint32_t *map = (uint32_t *) xpm_load(xpm, XPM_8_8_8_8, &img);

    if (map == NULL) {
        return 1;
    }

    for (int j = 0; j < img.height; j++) {
        for (int i = 0; i < img.width; i++) {

            if (vg_draw_pixel(x + i, y + j, map[i + j*img.width])) {
                return 1;
            }
        }
    }

    return 0;
}

int (create_back_buffer)() {
    back_buffer = (uint8_t *) malloc (h_res * v_res * bytes_per_pixel);

    if (back_buffer == NULL) {
        return 1;
    }

    memset(back_buffer, 0xFFFFFF, h_res * v_res * bytes_per_pixel);

    return 0;
}

void (copy_back_to_frame)() {
    memcpy(frame_adress, back_buffer, h_res * v_res * bytes_per_pixel);
}

void (erase_back_buffer)() {
    memset(back_buffer, 0xFFFFFF, h_res * v_res * bytes_per_pixel); 
}

void (destroy_back_buffer)() {
    free(back_buffer);
}
