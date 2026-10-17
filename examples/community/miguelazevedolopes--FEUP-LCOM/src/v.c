#pragma once
#include <lcom/lcf.h>
#include <stdint.h>
#include <stdio.h>
#include <machine/int86.h>
#include "v.h"
#include "keyboard.h"

#define VIDEO_CARD_INT 0x10  //BIOS call to specify video card service
#define VBE_LINEAR_FRAMEBUFFER BIT(14)
#define VBE_SET_MODE 0x4F02


//static void *video_mem2;         /* frame-buffer VM address (static global variable*/ 
static char *video_mem;		/* Process (virtual) address to which VRAM is mapped */
static unsigned bits_per_pixel; /* Number of VRAM bits per pixel */
static char *second_buffer;
static unsigned vram_size;

/* Public global variables */
static unsigned x_resolution;  /* Horizontal screen resolution in pixels */
static unsigned y_resolution; /* Vertical screen resolution in pixels */


void*(vg_init)(uint16_t mode) {
    struct reg86 r86;

    memset(&r86, 0, sizeof(r86));	
    r86.ax = VBE_SET_MODE;
    r86.bx = VBE_LINEAR_FRAMEBUFFER | mode;
    r86.intno = VIDEO_CARD_INT;
    
    if (sys_int86(&r86) != 0 ) {
        printf("\tvg_exit(): sys_int86() failed \n");
        return NULL;
    }

    if(map_vm(mode) == NULL) {
      return NULL;
    }
    return NULL;
}

int(vg_draw_rectangle)(	uint16_t 	x, uint16_t y, uint16_t 	width, uint16_t 	height, uint32_t 	color )	{

    size_t xf = x+width;
    size_t yf = y+height;
    /*for (size_t xi = x; xi <= xf; xi++) {
       for (size_t yi = y; yi <= yf; yi++){
            vg_colour_pixel(xi,yi, color);
        }
        
    }*/
    for (size_t yi = y; yi < yf; yi++)
    {
        vg_draw_hline(x,yi, xf,color);
    }
    
    
    return 0;
}

void*( map_vm)(uint16_t mode) {
    vbe_mode_info_t info;
    if(vbe_get_mode_info(mode, &info) != 0){
      return NULL;
    }
    x_resolution = info.XResolution;
    y_resolution = info.YResolution;
    bits_per_pixel = info.BitsPerPixel;

    unsigned int vram_base;  /* VRAM's physical addresss */
    //unsigned int vram_size;  /* VRAM's size, but you can use the frame-buffer size, instead */
    vram_size = x_resolution * y_resolution *((bits_per_pixel+7)/8);
    struct minix_mem_range mem_range;
    vram_base = info.PhysBasePtr;
    mem_range.mr_base = (phys_bytes)vram_base;
    mem_range.mr_limit = vram_base + vram_size;
    int r;	

/* Use VBE function 0x01 to initialize vram_base and vram_size */


/* Allow memory mapping */


    if( OK != (r = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mem_range))) {
        panic("sys_privctl (ADD_MEM) failed: %d\n", r);
        return NULL;
    }

/* Map memory */

    video_mem = vm_map_phys(SELF, (void *)mem_range.mr_base, vram_size);

    if(video_mem == MAP_FAILED) {
        panic("couldn't map video memory");
        return NULL;
    }
    second_buffer = malloc(vram_size);


    //return video_mem;~
    return NULL;
}

void (vg_colour_pixel)(uint16_t x,  uint16_t y, uint32_t color){
    if (x < x_resolution && x >= 0 && y < y_resolution && y>= 0){
        uint32_t pixel_position = (x + x_resolution * y) * ((bits_per_pixel+7)/8);

    
        memcpy(video_mem + pixel_position, &color,(bits_per_pixel+7)/8 );

            //Using palette mode
        /*    if(bits_per_pixel == 8) {
                video_mem[pixel_position] = (uint8_t) color;
            } else {
            //Using rgb mode
            uint32_t * base_video_mem = (uint32_t *) video_mem;
            base_video_mem[pixel_position] = color;
        }*/
    }
}

int(vg_draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color) {
    for (size_t xi = x; xi < len; xi++)
    {
        vg_colour_pixel(xi, y, color);
    }
    return 1;
    
}

void (vg_draw_xpm)(uint8_t *pixmap, xpm_image_t img, uint16_t x, uint16_t y) {
    int width = img.width;
    int height = img.height;
    for (int dy = 0; dy < height; dy++)
    {
        for (int  dx = 0; dx < width; dx++)
        {
            vg_colour_pixel(x+dx, y + dy, pixmap[dx + width*dy]);

        }
        
    }
}

bool (is_vertical_movement)(uint16_t xi, uint16_t yi, uint16_t xf, uint16_t yf) {
    return xi == xf && yi !=yf;
}


int (direction)(uint16_t si, uint16_t sf) {
    
    if (sf > si) {
        return 1;
    }
    return -1;
    
}

void (move_xmp)(uint16_t *xi, uint16_t *yi, uint16_t xf, uint16_t yf, uint16_t vx, uint16_t vy, uint16_t speed, uint8_t *pixmap, xpm_image_t img) {
    *xi += vx;
    *yi += vy;
    vg_draw_xpm(pixmap, img, *xi, *yi);
}

void (flip_buffers)() {
    memcpy(video_mem, second_buffer, vram_size);
}

