#include "gpu.h"

vbe_mode_info_t mode_info;
uint8_t *first_frame_buffer;
uint8_t *second_frame_buffer;
unsigned int frame_size;

int set_text_mode(){
    reg86_t reg86;
    memset(&reg86,0,sizeof(reg86_t));
    reg86.ax = 0x0003;
    reg86.bx = 0x0000;
    reg86.intno = 0x10;
    if (sys_int86(&reg86)!=0) return 1;
    return 0;
}

int set_graphic_mode(uint16_t submode){
    reg86_t reg86;
    memset(&reg86,0,sizeof(reg86_t));
    reg86.ax = 0x4F02;
    reg86.bx = submode | BIT(14);
    reg86.intno = 0x10;
    if (sys_int86(&reg86)!=0) return 1;
    return 0;
}

int set_frame_buffer(uint16_t mode){
    memset(&mode_info, 0, sizeof(mode_info));           // inicialmente todos os valores são 0
    if (vbe_get_mode_info(mode, &mode_info)) return 1;  // preenchimento dos valores

    uint8_t bytesPerPixel = (mode_info.BitsPerPixel+7)/8;
    frame_size = mode_info.XResolution*mode_info.YResolution*bytesPerPixel;

    struct minix_mem_range physicAdresses;
    physicAdresses.mr_base=mode_info.PhysBasePtr;
    physicAdresses.mr_limit=mode_info.PhysBasePtr+frame_size;

    if (sys_privctl(SELF,SYS_PRIV_ADD_MEM,&physicAdresses)!=0) return 1;

    first_frame_buffer = vm_map_phys(SELF, (void*) physicAdresses.mr_base, frame_size);
    if (first_frame_buffer==NULL) return 1;
    second_frame_buffer = (uint8_t *) malloc(frame_size);
    if (second_frame_buffer==NULL) return 1;

    return 0;
 }

int draw_frame_buffer(){
    memcpy(first_frame_buffer,second_frame_buffer,frame_size);
    memset(second_frame_buffer,0x000000,frame_size);
    return 0;
}
