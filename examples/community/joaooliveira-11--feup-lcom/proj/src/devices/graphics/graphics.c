#include "graphics.h"

int (changeTo_graphic_mode)(uint16_t submode){
    reg86_t r86;
    memset(&r86, 0, sizeof(r86));
    r86.ax = 0x4F02;
    r86.bx = submode | BIT(14);
    r86.intno = 0x10;
    if(sys_int86(&r86) != 0){
        printf("Error while trying to change to graphics mode\n");
        return 1;
    }
    return 0;
}    

int (frame_buffer_build)(uint16_t mode, uint8_t** buffer){

    memset(&vbe_mode_info, 0, sizeof(vbe_mode_info));
    if(vbe_get_mode_info(mode, &vbe_mode_info) != 0) return 1;

    unsigned int bytesPerPixel = (vbe_mode_info.BitsPerPixel + 7) / 8;  //converter o número de bits por pixel em bytes
    unsigned int frame_buffer_size = vbe_mode_info.XResolution * vbe_mode_info.YResolution * bytesPerPixel;  // tamanho da memoria da VRAM
    
    struct minix_mem_range frameBuffer_physic_address;
    frameBuffer_physic_address.mr_base = vbe_mode_info.PhysBasePtr;   // endereço físico base do frame buffer
    frameBuffer_physic_address.mr_limit = frameBuffer_physic_address.mr_base + frame_buffer_size; // endereço físico final do frame buffer

    if(sys_privctl(SELF, SYS_PRIV_ADD_MEM, &frameBuffer_physic_address)) {
        printf("Error while allocating physical memory for frame buffer\n");
        return 1;
    }

    *buffer = vm_map_phys(SELF, (void*) frameBuffer_physic_address.mr_base, frame_buffer_size);
    if (buffer == NULL) {
        printf("Error while allocating virtual memory for frame buffer\n");
        return 1;
    }

    return 0;
}

int (color_pixel)(uint16_t x, uint16_t y, uint32_t color, uint8_t* frame_buffer){

    if(x >= vbe_mode_info.XResolution || x < 0 ||  y >= vbe_mode_info.YResolution || y < 0) return 1;
    unsigned int bytesPerPixel = (vbe_mode_info.BitsPerPixel + 7) / 8;

    unsigned int frame_buffer_index = (vbe_mode_info.XResolution * y + x) * bytesPerPixel; // zona do frame buffer onde começa o pixel (x, y)

    if(memcpy(frame_buffer + frame_buffer_index, &color, bytesPerPixel) == NULL) return 1;  // copiar bytesPerPixel bytes da color na zona do frame buffer onde começa o pixel (x, y)

    return 0;   
}

