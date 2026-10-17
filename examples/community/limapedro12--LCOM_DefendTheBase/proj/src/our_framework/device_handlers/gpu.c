#include "gpu.h"

int vram_size = 0;
char * video_mem = NULL;
vbe_mode_info_t info;

void* (vg_init)(u16_t mode){
    NO_BACKGROUND = 0xFFFFFFFF;
    // Get Info
    vbe_get_mode_info(mode, &info);

    // Get Phisical Address to Virtual Space
    long bytes_per_pixel = info.BitsPerPixel/8 + (info.BitsPerPixel%8 != 0);
    unsigned int vram_base = info.PhysBasePtr;	
    vram_size = info.XResolution * info.YResolution * bytes_per_pixel;  

    struct minix_mem_range mr;
    mr.mr_base = (phys_bytes) vram_base;
    mr.mr_limit = mr.mr_base + vram_size*2;

    int response;

    if( OK != (response = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr)))
        panic("sys_privctl (ADD_MEM) failed: %d\n", response);

    /* Map memory */

    video_mem = vm_map_phys(SELF, (void* ) mr.mr_base, vram_size);

    memset(video_mem,0,vram_size);

    if(video_mem == MAP_FAILED)
        panic("couldn't map video memory");


    // Set VBE Mode
    reg86_t r;
    memset(&r, 0, sizeof(r));
    r.ax = 0x4F02; // VBE call, function 02 -- set VBE mode
    r.bx = 1<<14 | mode; // set bit 14: linear framebuffer
    r.intno = 0x10;
    if( sys_int86(&r) != OK ) {
        printf("set_vbe_mode: sys_int86() failed \n");
        return NULL;
    }
    double_buffer = malloc(vram_size);
    return video_mem;
}

void draw_pixel(int x, int y, uint color){  
  int bpp = 3; 
  if(x < 0 || x > info.XResolution || y < 0 || y > info.YResolution)
    return;
  for(int k = 0; k < bpp; k++)
    double_buffer[y*info.XResolution*bpp + x*bpp + k] = (color >> (k*8));
}

void draw_rect(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color, uint16_t mode){
    for(int i = x; i < x+width; i++)
        for(int j = y; j < y+height; j++)
            draw_pixel(i, j, color);
}

int doublebuffer_to_vram(){
    memcpy(video_mem, double_buffer, vram_size);
    return 0;
}

int get_width(uint16_t mode){
    vbe_mode_info_t info;
    vbe_get_mode_info(mode, &info);
    return info.XResolution;
}

int get_height(uint16_t mode){
    vbe_mode_info_t info;
    vbe_get_mode_info(mode, &info);
    return info.YResolution;
}

int gpu_exit(){
    free(double_buffer);
    return vg_exit();
}

int draw_xpm(uint16_t x, uint16_t y, xpm_map_t xpm, uint color_to_ignore){
    xpm_image_t img;
    uint8_t * pixmap = xpm_load(xpm, XPM_8_8_8, &img);
    for(int i = 0; i < img.height; i++)
        for(int j = 0; j < img.width; j++){
            unsigned int color = 0;
            for(int k = 0; k < 3; k++){
                color += ((uint) pixmap[i*img.width*3 + j*3 + k]) << (k*8);
            }
            if(color != color_to_ignore)
                draw_pixel(x+j, y+i, color);
            }
    
    return 0;
}

int draw_xpm_loaded(uint16_t x, uint16_t y, uint8_t * pixmap, xpm_image_t img, uint color_to_ignore){
    for(int i = 0; i < img.height; i++)
        for(int j = 0; j < img.width; j++){
            unsigned int color = 0;
            for(int k = 0; k < 3; k++){
                color += ((uint) pixmap[i*img.width*3 + j*3 + k]) << (k*8);
            }
            if(color != color_to_ignore)
                draw_pixel(x+j, y+i, color);
            }
    
    return 0;
}

int get_vram_size(){
    return vram_size;
}
