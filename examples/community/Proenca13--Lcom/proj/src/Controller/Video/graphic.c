//
// Created by joao on 14-04-2024.
//
#include <lcom/lcf.h>
#include "graphic.h"
vbe_mode_info_t modeInfo;
uint8_t* frame_buffer;
uint8_t *second_frame_buffer;
uint8_t *drawing_frame_buffer;
unsigned int vram_size;
int (set_graphic_mode)(uint16_t mode){
    reg86_t r;
    memset(&r ,0, sizeof(r));
    r.ax = 0x4F02; // VBE call, function 02 -- set VBE mode
    r.bx = BIT(14)|mode; // set bit 14: linear framebuffer
    r.intno = VBE;
    if( sys_int86(&r) != 0 ) {
        printf("set_vbe_mode: sys_int86() failed \n");
        return 1;
    }
    return 0;
}
int (set_frame_buffer)(uint16_t mode){
    memset(&modeInfo ,0, sizeof(modeInfo));
    if(vbe_get_mode_info(mode, &modeInfo)!=0)return 1;

    unsigned int bytes_per_pixel = (modeInfo.BitsPerPixel+7)/8;
    vram_size = modeInfo.XResolution * modeInfo.YResolution * bytes_per_pixel;;

    struct minix_mem_range mr;
    mr.mr_base = modeInfo.PhysBasePtr;
    mr.mr_limit = mr.mr_base + vram_size;

    if(  sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr)!=0){
        return 1;
    }

    frame_buffer = vm_map_phys(SELF, (void *)mr.mr_base, vram_size);
    if(frame_buffer == NULL){
        panic("couldn’t map video memory");
        return 1;
    }
    second_frame_buffer = (uint8_t *) malloc(vram_size);
    drawing_frame_buffer = second_frame_buffer;
    return 0;
}
void swap_buffers() {
    memcpy(frame_buffer, drawing_frame_buffer, vram_size);
}
int vg_draw_pixel(uint16_t x, uint16_t y, uint32_t color){
    if(x >= modeInfo.XResolution || y >= modeInfo.YResolution || x <0 || y <0) return 1;
    unsigned int bytes_per_pixel = (modeInfo.BitsPerPixel+7)/8;
    unsigned int index = (modeInfo.XResolution * y +x)*bytes_per_pixel;
    if (memcpy(&drawing_frame_buffer[index], &color, bytes_per_pixel) == NULL) return 1;
    return 0;
}
int (vg_draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color){
    for (unsigned i = 0 ; i < len ; i++)
        if (vg_draw_pixel(x+i, y, color) != 0) return 1;
    return 0;
}
int (vg_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color){
    for(unsigned i = 0; i < height ; i++)
        if (vg_draw_hline(x, y+i, width, color) != 0) {
            return 1;
        }
    return 0;
}
int (print_xpm)(xpm_map_t xpm, uint16_t x, uint16_t y){
    xpm_image_t img;
    uint8_t *temp_colormap = xpm_load(xpm, XPM_8_8_8_8, &img);
    uint32_t *colormap = (uint32_t *)temp_colormap;
    for(int h = 0;h <img.height;h++){
        for(int w = 0; w < img.width;w++){
            if(vg_draw_pixel(w+x,h+y,*colormap)!=0)return 1;
            colormap++;
        }
    }

    return 0;
}
int normalize_color(uint32_t color, uint32_t *new_color) {
    if (modeInfo.BitsPerPixel == 32) {
        *new_color = color;
    } else {
        *new_color = color & (BIT(modeInfo.BitsPerPixel) - 1);
    }
    return 0;
}
