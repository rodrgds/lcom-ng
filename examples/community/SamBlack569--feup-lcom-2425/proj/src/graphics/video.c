#include <lcom/lcf.h>
#include "video.h"
#include "xpms/mainMenu.h"
#include "xpms/background.h"
#include <machine/int86.h>

void *video_mem;
uint8_t *double_buffer;
uint8_t *background_buffer;
vbe_mode_info_t vmi_p;

unsigned hres;
unsigned vres;

int(vg_enter)(uint16_t mode) {
    reg86_t r86;
   
    /* Specify the appropriate register values */
    
    memset(&r86, 0, sizeof(r86));	/* zero the structure */

    r86.intno = 0x10; /* BIOS video services */
    r86.ah = 0x4F;    /* Set Video Mode function */
    r86.al = 0x02;    /* 80x25 text mode */
    r86.bx = mode | BIT(14);    /* mode to set */
    
    /* Make the BIOS call */

    if( sys_int86(&r86) != OK ) {
        printf("\tvg_exit(): sys_int86() failed \n");
        return 1;
    }

    return 0;
}

int(vg_map_vram)(uint16_t mode) {
    struct minix_mem_range mr;
    unsigned int vram_base;
    unsigned int vram_size;

    int r;

    /* Use VBE function 0x01 to initialize vram_base and vram_size */

    if (vbe_get_mode_info(mode, &vmi_p) != OK) {
        return 1;
    }

    vram_base = vmi_p.PhysBasePtr;

    hres = vmi_p.XResolution;
    vres = vmi_p.YResolution;

    int bytes_per_pixel = (vmi_p.BitsPerPixel + 7) / 8;
    vram_size = hres * vres * bytes_per_pixel;
    
    
    mr.mr_base = (phys_bytes) vram_base;
    mr.mr_limit = vram_base + vram_size;

    if( OK != (r = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr)))
        panic("sys_privctl (ADD_MEM) failed: %d\n", r);
    
    /* Map memory */

    video_mem = vm_map_phys(SELF, (void *)mr.mr_base, vram_size);

    if(video_mem == MAP_FAILED)
        panic("couldn't map video memory");

    double_buffer = malloc(vram_size);
    if(double_buffer == NULL)
        panic("couldn't allocate memory for double buffer");

    background_buffer = malloc(vram_size);
    if(background_buffer == NULL)
        panic("couldn't allocate memory for background buffer");

    return 0;
}

int(vg_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color) {
    
    for(int i=0; i<height; i++){
        if(vg_draw_hline(x, y+i, width, color) != 0){
            return 1;
        }
    }

    return 0;
}

int(vg_draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color) {
    
    for(int i=0; i<len; i++){
        if(vg_draw_pixel(x+i, y, color, double_buffer) != 0){
            return 1;
        }
    }

    return 0;
}

int(vg_draw_pixel)(uint16_t x, uint16_t y, uint32_t color, uint8_t* buffer) {
    if (x >= hres || y >= vres) {
        return 1;
    }
    uint8_t bytes_per_pixel = (vmi_p.BitsPerPixel + 7) / 8;
    uint8_t *pixel_address = buffer + (y * hres + x) * bytes_per_pixel;

    if (vmi_p.BitsPerPixel == 8) { // Indexed color mode
        *pixel_address = (uint8_t)color;
    } 
    else {  // Direct color mode
        memcpy(pixel_address, &color, bytes_per_pixel);
    }

    return 0;
}

int(vg_draw_pattern)(uint8_t no_rectangles, uint32_t first, uint8_t step) {
    uint32_t color = 0x000;
    uint16_t width_rectangle = hres / no_rectangles;
    uint16_t height_rectangle = vres / no_rectangles;

    for(int row=0; row<no_rectangles; row++){
        for(int col=0; col<no_rectangles; col++){
            if(vmi_p.BitsPerPixel == 8) {   //index mode
                color = (first + (row * no_rectangles + col) * step) % (1 << vmi_p.BitsPerPixel);
            }
            else {  //direct color mode
                
                uint32_t red_part = (first >> vmi_p.RedFieldPosition) & ((1 << vmi_p.RedMaskSize) - 1);
                uint32_t green_part = (first >> vmi_p.GreenFieldPosition) & ((1 << vmi_p.GreenMaskSize) - 1);
                uint32_t blue_part = (first >> vmi_p.BlueFieldPosition) & ((1 << vmi_p.BlueMaskSize) - 1);
                
                uint32_t red = (red_part + col * step) % (1 << vmi_p.RedMaskSize);
                uint32_t green = (green_part + row * step) % (1 << vmi_p.GreenMaskSize);
                uint32_t blue = (blue_part + (col + row) * step) % (1 << vmi_p.BlueMaskSize);
        
                color = (red << vmi_p.RedFieldPosition) | (green << vmi_p.GreenFieldPosition) | (blue << vmi_p.BlueFieldPosition);
            }

            if(vg_draw_rectangle(col*width_rectangle, row*height_rectangle, width_rectangle, height_rectangle, color) != 0){
                return 1;
            }
        }        
    }

    return 0;
}


int(vg_draw_xpm)(xpm_map_t xpm, uint16_t x, uint16_t y, uint8_t *buffer) {
    xpm_image_t img;
    uint8_t *xpm_map = xpm_load(xpm, XPM_8_8_8_8, &img);

    if (xpm_map == NULL) {
        printf("Failed to load XPM image\n");
        return 1;
    }

    for (int i = 0; i < img.height; i++) {
        for (int j = 0; j < img.width; j++) {
            uint32_t color = ((uint32_t *)xpm_map)[i * img.width + j];
            
            if (((color >> 24) & 0xFF) == 0xFF) //skip transparent color
                continue;

            if (vg_draw_pixel(x + j, y + i, color, buffer) != 0) {
                printf("Error drawing pixel at (%d,%d)\n", x + j, y + i);
                return 1;
            }
        }
    }

    return 0;
}

void(load_buffer)() {
    memcpy(video_mem, double_buffer, hres * vres * ((vmi_p.BitsPerPixel + 7) / 8));
}

void(load_menu_background)() {
    vg_draw_xpm(mainMenu_xpm, 0, 0, background_buffer);
}

void(get_menu)() {
    memcpy(double_buffer, background_buffer, hres * vres * ((vmi_p.BitsPerPixel + 7) / 8));
}

void(get_game_background)() {
    memcpy(double_buffer, background_buffer, hres * vres * ((vmi_p.BitsPerPixel + 7) / 8));
}

void(clear_back_buffer)() {
    memset(background_buffer, 0, hres * vres * ((vmi_p.BitsPerPixel + 7) / 8));
}

void(clear_double_buffer)() {
    memset(double_buffer, 0, hres * vres * ((vmi_p.BitsPerPixel + 7) / 8));
}
void(clear_all_buffers)(){
    memset(video_mem, 0, hres * vres * ((vmi_p.BitsPerPixel + 7) / 8));
    memset(double_buffer, 0, hres * vres * ((vmi_p.BitsPerPixel + 7) / 8));
    memset(background_buffer, 0, hres * vres * ((vmi_p.BitsPerPixel + 7) / 8));
}

void(load_game_background)() {
    vg_draw_xpm(background_xpm, 0, 0, background_buffer);
}

