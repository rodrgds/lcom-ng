#include "graphics.h"
#include "macros.h"
#include "string.h"

static unsigned int vram_size;

int setMode(uint16_t mode){

    reg86_t r;
    memset(&r, 0, sizeof(r));
    r.intno = 0x10;
    r.ah = 0x4F;
    r.al = 0x02;
    r.ax = 0x4F02;
    r.bx = 1<<14|mode; 
    if( sys_int86(&r) != OK ) {
        printf("set_vbe_mode: sys_int86() failed \n");
        return 1;
    }
    return 0;
}


void vramMap(){
    int r;
    struct minix_mem_range mr; /* physical memory range */
    unsigned int vram_base=vmi_p.PhysBasePtr; /* VRAM’s physical addresss */
    vram_size=vmi_p.XResolution *vmi_p.YResolution* (vmi_p.BitsPerPixel +7) >> 3; /* VRAM’s size, but you can use
    the frame-buffer size, instead */
    /* Allow memory mapping */
    mr.mr_base = (phys_bytes) vram_base;
    mr.mr_limit = mr.mr_base + vram_size;
    if( OK != (r = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr)))
        panic("sys_privctl (ADD_MEM) failed: %d\n", r);
    /* Map memory */
    video_mem = vm_map_phys(SELF, (void *)mr.mr_base, vram_size);
    if(video_mem == MAP_FAILED)
        panic("couldn’t map video memory");
    bits_per_pixel=vmi_p.BitsPerPixel;
    h_res=vmi_p.XResolution;
    v_res= vmi_p.YResolution;
    bytes_per_pixel=(vmi_p.BitsPerPixel) >> 3;


    /* Map video mem Buffer */
    mr.mr_base = (phys_bytes) vram_base + vram_size;
    mr.mr_limit = mr.mr_base + vram_size;
    if( OK != (r = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr)))
        panic("sys_privctl (ADD_MEM) failed: %d\n", r);
    
    video_mem_buf = vm_map_phys(SELF, (void *)mr.mr_base, vram_size);
    if(video_mem_buf == MAP_FAILED)
        panic("couldn’t map video memory");
    
    
    
}

void draw_xpm(xpm_image_t img, uint8_t *map, int x, int y){

    uint32_t transparentColor = xpm_transparency_color(img.type);
    for(unsigned row = 0; row < img.height && y + row < vmi_p.YResolution; row++){
        for(unsigned col=0; col < img.width && x + col < vmi_p.XResolution; col++){

            unsigned int p = (x+ col + (y + row) * h_res) * bytes_per_pixel;
            unsigned color_position = (row*img.width+ col)*bytes_per_pixel;
            
            if (memcmp( &img.bytes[color_position] , (char*)&transparentColor, bytes_per_pixel) == 0 )
                continue;
            
            memcpy((void *)((uintptr_t)video_mem_buf + p), &img.bytes[color_position], bytes_per_pixel);
                
        }
    }
}


void (draw_xpm_video_mem)(xpm_image_t img, uint8_t *map, int x, int y){
    uint32_t transparentColor = xpm_transparency_color(img.type);
    for(unsigned row = 0; row < img.height && y + row < vmi_p.YResolution; row++){
        for(unsigned col=0; col < img.width && x + col < vmi_p.XResolution; col++){

            unsigned int p = (x+ col + (y + row) * h_res) * bytes_per_pixel;
            unsigned color_position = (row*img.width+ col)*bytes_per_pixel;
            
            if (memcmp( &img.bytes[color_position] , (char*)&transparentColor, bytes_per_pixel) == 0 )
                continue;
            
            memcpy((void *)((uintptr_t)video_mem + p), &img.bytes[color_position], bytes_per_pixel);
                
        }
    }
}

void erase_xpm(xpm_image_t img, int x, int y, xpm_image_t background){
    for(uint8_t col = 0; col < img.width; col++){
        for(uint8_t row = 0; row < img.height; row++){
            unsigned int p = (x+ col + (y + row) * h_res) * bytes_per_pixel;
            unsigned color_position = ((y+row)*background.width+ (x+col))*bytes_per_pixel;
            memcpy((void *)((uintptr_t)video_mem_buf + p), &background.bytes[color_position], bytes_per_pixel);
        }
    }
}

void copy_buffer_to_mem(){
    memcpy(video_mem, video_mem_buf, vram_size);
}

