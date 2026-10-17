#include <lcom/lab5.h>
#include <lcom/lcf.h>

#include "video.h"

char *video_mem;

unsigned h_res;
unsigned v_res;
unsigned bits_per_pixel;

char* double_buffer = NULL;

int (vbe_set_mode)(uint16_t mode){
    reg86_t r86;

    memset(&r86, 0, sizeof(r86));

    r86.intno = INTNO;
    r86.ah = AH_GRAPHIC_MODE;
    r86.al = AL_GRAPHIC_MODE;

    r86.bx = mode | LINEAR_FB;

    if(sys_int86(&r86) != 0){
        printf("\tvg_exit(): sys_int86() failed \n");
        return 1;
    }
    return 0;
}

int (frame_buffer_init)(uint16_t mode) {
    struct minix_mem_range mr;
    unsigned int vram_base;
    unsigned int vram_size;
    int r;				    

    vbe_mode_info_t vmi;
    if (vbe_get_mode_info(mode, &vmi) != 0) return 1;

    vram_base = vmi.PhysBasePtr;
    vram_size = vmi.XResolution * vmi.YResolution * ((vmi.BitsPerPixel + 7) / 8);

    mr.mr_base = vram_base;
    mr.mr_limit = mr.mr_base + vram_size;

    if((r = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr)) != 0){
        panic("sys_privctl (ADD_MEM) failed: %d\n", r);
    }

    video_mem = vm_map_phys(SELF, (void *)mr.mr_base, vram_size);
    if (video_mem == MAP_FAILED) {
        panic("couldn't map video memory");
    }

    double_buffer = (char*) malloc(vram_size);
    if (double_buffer == NULL) {
        panic("couldn't allocate memory for double buffer");
    }

    h_res = vmi.XResolution;
    v_res = vmi.YResolution;
    bits_per_pixel = vmi.BitsPerPixel;

    return 0;
}

int (vg_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color){
    for (uint16_t j = y; j < y + height; j++){
        vg_draw_hline(x,j,width,color);
    }
    return 0;
}

int (vg_draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color){
    for(int i=x; i < x+len; i++){
        uint32_t pixel_offset = (y * h_res + i) * (bits_per_pixel / 8);
        uint8_t* pixel_ptr = (uint8_t*) double_buffer + pixel_offset;

        color_pixel(pixel_ptr, color);
    }
    return 0;
}

int (vg_draw_vline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color){
    for(int i=y; i < y+len; i++){
        uint32_t pixel_offset = (i * h_res + x) * (bits_per_pixel / 8);
        uint8_t* pixel_ptr = (uint8_t*) double_buffer + pixel_offset;

        color_pixel(pixel_ptr, color);
    }
    return 0;
}

int (color_pixel)(uint8_t* pixel_ptr, uint32_t color){
    switch (bits_per_pixel)
    {
    case 8:
        *pixel_ptr = (uint8_t) color;
        break;
    case 15:
        *(uint16_t*) pixel_ptr = (uint16_t) color;
        break;
    case 24:
        pixel_ptr[0] = (uint8_t) color; // blue
        pixel_ptr[1] = (uint8_t) (color >> 8); // green
        pixel_ptr[2] = (uint8_t) (color >> 16); // red
        break;
    case 16:
        *(uint16_t*) pixel_ptr = (uint16_t) color;
        break;
    case 32:
        *(uint32_t*) pixel_ptr = color;
        break;
    default:
        return 1;
    }
    return 0;
}

int (vg_draw_xpm)(uint16_t x, uint16_t y, const xpm_image_t *img) {
    uint8_t *colors = img->bytes;

    for (uint16_t row = 0; row < img->height; row++) {
        for (uint16_t col = 0; col < img->width; col++) {
            uint8_t b = *colors++;
            uint8_t g = *colors++;
            uint8_t r = *colors++;

            uint32_t color = (r << 16) | (g << 8) | b;

            uint16_t pixel_x = x + col;
            uint16_t pixel_y = y + row;

            if (pixel_x < h_res && pixel_y < v_res) {
                uint32_t pixel_offset = (pixel_y * h_res + pixel_x) * (bits_per_pixel / 8);
                uint8_t* pixel_ptr = (uint8_t*) double_buffer + pixel_offset;

                color_pixel(pixel_ptr, color);
            }
        }
    }
    return 0;
}

int (vg_clear_screen)() {
    if (double_buffer == NULL) return 1;

    uint32_t beige_color = (245 << 16) | (245 << 8) | 220; // Nossa cor de fundo : #F5F5DC

    for (unsigned y = 0; y < v_res; y++) {
        for (unsigned x = 0; x < h_res; x++) {
            uint32_t offset = (y * h_res + x) * (bits_per_pixel / 8);
            uint8_t *pixel_ptr = (uint8_t *)double_buffer + offset;
            color_pixel(pixel_ptr, beige_color);
        }
    }

    return 0;
}

void (swap_buffers)() {
    memcpy(video_mem, double_buffer, h_res * v_res * (bits_per_pixel / 8));
}


  
