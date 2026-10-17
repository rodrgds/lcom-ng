// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>
#include <stdint.h>
#include <stdio.h>

#include "video.h"

vbe_mode_info_t vmi_p;
static char *video_mem;
static unsigned memory_model;
static unsigned h_res; /* Horizontal resolution in pixels */
static unsigned v_res; /* Vertical resolution in pixels */
static unsigned red_pos, green_pos, blue_pos; /* bit position of lsb of red, green and blue mask */
static unsigned red_size, green_size, blue_size; /* size of direct color red, green and blue mask in bits */
static unsigned bytes_per_pixel;
static unsigned bits_per_pixel;
static unsigned int vram_size;

static char *buffer;


void *(vg_init)(uint16_t mode){

    struct minix_mem_range mr;
    unsigned int vram_base;

    int r;

    my_vbe_get_mode_info(mode,&vmi_p);
    vram_base = vmi_p.PhysBasePtr;
    memory_model = vmi_p.MemoryModel;

    v_res = vmi_p.YResolution;
    h_res = vmi_p.XResolution;

    red_pos = vmi_p.RedFieldPosition; red_size = vmi_p.RedMaskSize;
    green_pos = vmi_p.GreenFieldPosition; green_size = vmi_p.GreenMaskSize;
    blue_pos = vmi_p.BlueFieldPosition; blue_size = vmi_p.BlueMaskSize;

    bits_per_pixel = vmi_p.BitsPerPixel;
    bytes_per_pixel = (bits_per_pixel + ((8 - bits_per_pixel) % 8)) / 8;

    vram_size = bytes_per_pixel * v_res * h_res;

    mr.mr_base = (phys_bytes) vram_base;	
    mr.mr_limit = mr.mr_base + vram_size;  

    if( OK != (r = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr)))
        panic("sys_privctl (ADD_MEM) failed: %d\n", r);

    video_mem = vm_map_phys(SELF, (void *)mr.mr_base, vram_size);

    if(video_mem == MAP_FAILED)
        panic("couldn't map video memory");

    reg86_t reg86;
    memset(&reg86,0,sizeof(reg86_t));

    reg86.ax = 0x4F02;
    reg86.intno = 0x10;
    reg86.bx = 1 << 14 | mode;

    if(sys_int86(&reg86) != 0) return NULL;

    buffer = malloc(vram_size);

    return video_mem;

    

}

int (my_vbe_get_mode_info)(uint16_t mode, vbe_mode_info_t *vmi_p){

    mmap_t map;
    if(lm_alloc(sizeof(vbe_mode_info_t), &map) == NULL) return 1;

    reg86_t reg;
    memset(&reg, 0, sizeof(reg86_t));

    reg.intno = 0x10;
    reg.ax = 0x4F01;
    reg.es = PB2BASE(map.phys);
    reg.di = PB2OFF(map.phys);
    reg.cx = mode;

    if(sys_int86(&reg) != 0) return 1;

    *vmi_p = *((vbe_mode_info_t*) map.virt);

    if(!lm_free(&map)) return 1;

    return 0;
}

int (vg_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color){

    for(int i = 0; i < height; i++){
        if(vg_draw_hline(x,y + i, width, color) != 0) return 1;
    }
    return 0;
}

int (vg_draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color){

    for(int i = 0; i < len; i++){
        if(vg_draw_pixel(x + i, y, color) != 0) return 1;
    }
    return 0;
}


void (copy_buffer)(){
    memcpy(video_mem,buffer,vram_size);
}

int (vg_draw_pixel)(uint16_t x, uint16_t y, uint32_t color){
    if(x > vmi_p.XResolution || y > vmi_p.YResolution) return 1;
   
    memcpy(buffer + (vmi_p.XResolution * y + x) * bytes_per_pixel , &color, bytes_per_pixel);
    return 0;
}

int (vg_draw_sprite)(uint8_t *map, uint16_t x, uint16_t y, xpm_image_t img){

    uint32_t color, redColor, greenColor, blueColor;

    for(int i = 0; i < img.height; i++){
        for(int j = 0; j < img.width; j++){

            if(*(map) == 0x40 && *(map + 1) == 0xB1 && *(map + 2) == 0x00){
                map += 3;
                continue;
            }
            if(x + j >= 800 || y + i >= 600){
                map += 3;
                continue;
            }
            redColor = *(map + 2);
            greenColor = *(map + 1);
            blueColor = *(map);
            color = (redColor << 16) | (greenColor << 8) | blueColor;
            vg_draw_pixel(x + j, y + i,color);
            map += 3;
        }
    }


    return 0;
}

int (vg_draw_astro)(uint8_t *map, uint16_t x, uint16_t y, xpm_image_t img, uint8_t *bgMap){

    uint32_t color, redColor, greenColor, blueColor;

    for(int i = 0; i < img.height; i++){
        for(int j = 0; j < img.width; j++){
            
            if(*(map) == 0x40 && *(map + 1) == 0xB1 && *(map + 2) == 0x00){
                map += 3;
                continue;
            }

            uint8_t blueBuffer = *(buffer + 3*(h_res * (y + i) + (x + j))) & 0xFF;
            uint8_t greenBuffer = *(buffer + 3*(h_res * (y + i) + (x + j)) + 1) & 0xFF;
            uint8_t redBuffer = *(buffer + 3*(h_res * (y + i) + (x + j)) + 2) & 0xFF;
            
            if(blueBuffer  != (*(bgMap +(3*(h_res * (y + i) + (x + j)))) & 0XFF) || 
               greenBuffer != (*(bgMap +(3*(h_res * (y + i) + (x + j)) + 1)) & 0XFF)|| 
               redBuffer != (*(bgMap +(3*(h_res * (y + i) + (x + j)) + 2)) & 0XFF) ){
                   if(blueBuffer == 0xC8 && greenBuffer == 0xD7 && redBuffer == 0x6E) return 2;
                return 1;
            }
            
            
            redColor = *(map + 2);
            greenColor = *(map + 1);
            blueColor = *(map);
            color = (redColor << 16) | (greenColor << 8) | blueColor;
            vg_draw_pixel(x + j, y + i,color);
            map += 3;
        }
    }

    return 0;
}


int (vg_destroy_sprite)(uint16_t x, uint16_t y, xpm_image_t img, uint8_t *bgMap){

    uint32_t color, redColor, greenColor, blueColor;

    for(int i = 0; i < img.height; i++){
        for(int j = 0; j < img.width; j++){
            redColor = *(bgMap +(3*(h_res * (y + i) + (x + j)) + 2)) & 0xFF;
            greenColor = *(bgMap +(3*(h_res * (y + i) + (x + j)) + 1)) & 0xFF;
            blueColor = *(bgMap +(3*(h_res * (y + i) + (x + j)))) & 0xFF;
            color = (redColor << 16) | (greenColor << 8) | blueColor;
            vg_draw_pixel(x + j, y + i,color);
        }
    }

    return 0;
}

