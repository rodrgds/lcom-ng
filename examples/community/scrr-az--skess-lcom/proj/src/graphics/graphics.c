#include "graphics.h"
#include <lcom/lcf.h>
#include <machine/int86.h>

vbe_mode_info_t vbe_mode_info;
uint8_t* video_mem1;
uint8_t* video_mem2;
int current = 1;

void* (vg_init)(uint16_t mode){
    int one_buffer_size;
    video_mem1 = set_frame_buffer(mode, &one_buffer_size);

    if(video_mem1 == NULL) return NULL;

    video_mem2 = video_mem1 + one_buffer_size; //the second buffer starts after the first one (of size one_buffer_size)

    //set it to graphics mode
    reg86_t reg86;

    memset(&reg86, 0, sizeof(reg86)); //set everything to 0

    reg86.intno = INTNO;
    reg86.ah = VBE_AH;
    reg86.al = VBE_SET_MODE;
    reg86.bx = mode | VBE_BX_OR;

    if(sys_int86(&reg86)) return NULL;

    return video_mem1;
}

void* (set_frame_buffer)(uint16_t mode, int* one_buffer_size){

    if(one_buffer_size == NULL) return NULL;

    memset(&vbe_mode_info, 0, sizeof(vbe_mode_info)); //set everything to 0
    if(vbe_get_mode_info(mode, &vbe_mode_info)) return NULL; //get the mode info

    struct minix_mem_range mr;
    unsigned int vram_base = vbe_mode_info.PhysBasePtr; //base for the address space

    unsigned int bytesPerPixel = (vbe_mode_info.BitsPerPixel + 7) / 8; //if last byte is not complete, have to count it either way
    *one_buffer_size = vbe_mode_info.XResolution * vbe_mode_info.YResolution * bytesPerPixel;
    unsigned int vram_size = 2 * *one_buffer_size; //size needed for 2 buffers
    
    mr.mr_base = vram_base;
    mr.mr_limit = mr.mr_base + vram_size; //the limit of the address space is the base + the space it needs

    if(sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr)) return NULL; //allow memory to be mapped in that address

    uint8_t* mem = vm_map_phys(SELF, (void*) mr.mr_base, vram_size); //map the memory

    if(mem == MAP_FAILED) return NULL; //check if it was correctly mapped
    return mem;
}

int (vg_draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color){
    uint8_t* buffer = (current == 1 ? video_mem2 : video_mem1);

    for (uint16_t i = x; i<x+len; i++){
        ((uint32_t*)buffer)[y * vbe_mode_info.XResolution + i] = color;
    }

    return 0;
}

int (vg_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color){
    //check if the values are valid
    if (x<0 || y<0 || x>vbe_mode_info.XResolution || x+width>vbe_mode_info.XResolution || y>vbe_mode_info.YResolution || y+height>vbe_mode_info.YResolution) return 1;

    for(uint16_t i = y; i<y+height; i++){ //iterate through the y values
        
        if(vg_draw_hline(x, i, width, color)){
            vg_exit();
            return 1;
        } //draw an horizontal line
    }

    return 0;
}

int (vg_draw_xpm)(xpm_image_t img, uint8_t* map, uint16_t x, uint16_t y){
    uint8_t* buffer = (current == 1 ? video_mem2 : video_mem1);

    for (unsigned int i = 0; i < img.height; i++){ //iterate through rows
        if (y + i >= vbe_mode_info.YResolution) break;

        for (unsigned int j = 0; j < img.width; j++){ //iterate trough columns
            if (x + j >= vbe_mode_info.XResolution){
                map+=4;
                continue;
            }

            uint32_t color = (*map << 16) | (*(map+1) << 8) | *(map+2);

            map += 4; //advance the pointer to the next value of the array

            if(color == 128 << 8) continue;

            ((uint32_t*)buffer)[(y+i) * vbe_mode_info.XResolution + x + j] = color;
        }
    }

    return 0;
}

int (change_display_buffer)(){
    reg86_t reg86;

    memset(&reg86, 0, sizeof(reg86)); //set everything to 0

    reg86.intno = INTNO;
    reg86.ah = VBE_AH;
    reg86.al = VBE_DISPLAY_CTRL;
    reg86.bx = VBE_SET_DURING_VRETRACE;
    reg86.cx = 0;
    reg86.dx = current == 1 ? vbe_mode_info.YResolution : 0;

    if(sys_int86(&reg86)) return 1;

    current = current == 1? 2: 1;
    return 0;
}

int (clear_buffer)(){
    uint8_t* buffer = (current == 1 ? video_mem2 : video_mem1);
    uint32_t color = BACKGROUND_COLOR;
    unsigned int total_pixels = vbe_mode_info.XResolution * vbe_mode_info.YResolution;

    for (unsigned int i = 0; i < total_pixels; i++) {
        ((uint32_t*)buffer)[i] = color;
    }

    return 0;
}

int (vg_draw_buffer)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t* map){
    if (map == NULL) return 1;
    uint8_t* buffer = (current == 1 ? video_mem2 : video_mem1);

    for (unsigned int i = 0; i < height; i++){ //iterate through rows
        if (y + i >= vbe_mode_info.YResolution) break;

        for (unsigned int j = 0; j < width; j++){ //iterate trough columns
            uint32_t color = map[i * width + j];
            ((uint32_t*)buffer)[(y + i) * vbe_mode_info.XResolution + x + j] = color;
        }
    }

    return 0;
}
