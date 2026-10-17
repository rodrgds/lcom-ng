#include <lcom/lcf.h>
#include <machine/int86.h>

#include "math.h"
#include "video_gr.h"
#include "macro_video.h"

static unsigned h_res;	        /* Horizontal resolution in pixels */
static unsigned v_res;	        /* Vertical resolution in pixels */
static unsigned bpp;
static unsigned bits_per_pixel;
static unsigned int vram_base;
static unsigned int vram_size;
static bool indexed_mode;
static uint8_t red_mask_size;
static uint8_t green_mask_size;
static uint8_t blue_mask_size;

static char* main_fb;
static char* second_fb;


void (swap_buffers)() {
    memcpy(main_fb, second_fb, vram_size);
}

int (init_buffers)(uint16_t mode) {
    if (vg_init(mode) == NULL) return 1;
    second_fb = (char*) malloc (vram_size);
    return 0;
}


void *(vg_init)(uint16_t mode) {
    vbe_mode_info_t vmi_p;
    if (vbe_get_mode_info(mode, &vmi_p)) {
        return NULL;
    }


    h_res = vmi_p.XResolution;
    v_res = vmi_p.YResolution;
    bits_per_pixel = vmi_p.BitsPerPixel;
    bpp = ceil(vmi_p.BitsPerPixel / 8.0);
    vram_base = vmi_p.PhysBasePtr;
    vram_size = h_res * v_res * bpp;

    red_mask_size = vmi_p.RedMaskSize;
    green_mask_size = vmi_p.GreenMaskSize;
    blue_mask_size = vmi_p.BlueMaskSize;

    struct minix_mem_range mr;
    int error;

    /* Allow memory mapping */

    mr.mr_base = (phys_bytes) vram_base;	
    mr.mr_limit = mr.mr_base + vram_size;  

    if( OK != (error = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr))) {
        panic("sys_privctl (ADD_MEM) failed: %d\n", error);
        return NULL;    
    }

    main_fb = vm_map_phys(SELF, (void *)mr.mr_base, vram_size);

    if(main_fb == MAP_FAILED) {
        panic("couldn't map video memory");
        return NULL;
    }
    
    reg86_t r;

    memset(&r, 0, sizeof(r));

    r.ax = SET_VBE_MODE;
    r.bx = BIT(14) | mode;
    r.intno = 0x10; 
    if (sys_int86(&r) != OK) { 
        printf("set_vbe_mode: sys_int86() failed \n");

        return NULL; 
    }
    
    indexed_mode = (mode == 0x105);

    return main_fb;
}

int (vg_draw_point)(uint16_t x, uint16_t y, uint32_t color, uint8_t size) {
    for (uint8_t i = 0; i < size; i++) {
        for (uint8_t j = 0; j < size; j++) {
            uint16_t a = x+i > 795 ? 795 : x+i, b = y+j > 595 ? 595 : y+j;
            if (vg_draw_pixel(a, b, color, true)) return 1;
        }
    }
    return 0;
}

int (vg_draw_pixel)(uint16_t x, uint16_t y, uint32_t color, bool secondBuf) {
    char* pos;
    if (secondBuf) pos = (second_fb + (y * h_res + x) * bpp);
    else pos = (main_fb + (y * h_res + x) * bpp);
    if (memcpy(pos, &color, bpp) == NULL) return 1;
    return 0;
}

int sign(int16_t x) {
    return (x > 0) ? 1 : ((x < 0) ? -1 : 0);
}

int (free_second_buffer)() {
    free(second_fb); 
    return 0; 
}

int (vg_draw_line)(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint32_t color, uint8_t size) {
    int16_t dx, dy, p, x, y, incx, incy, abs_dx, abs_dy;
    
    dx = x1 - x0;
    dy = y1 - y0;
    abs_dx = abs(dx);
    abs_dy = abs(dy);
    incx = sign(dx);
    incy = sign(dy);
    
    x = x0;
    y = y0;
    
    vg_draw_point(x, y, color, size);
    
    if (abs_dx >= abs_dy) {
        p = 2 * abs_dy - abs_dx;
        for (int i = 0; i < abs_dx; i++) {
            if (p >= 0) {
                y += incy;
                p += 2 * abs_dy - 2 * abs_dx;
            } else {
                p += 2 * abs_dy;
            }
            x += incx;
            vg_draw_point(x, y, color, size);
        }
    } else {
        p = 2 * abs_dx - abs_dy;
        for (int i = 0; i < abs_dy; i++) {
            if (p >= 0) {
                x += incx;
                p += 2 * abs_dx - 2 * abs_dy;
            } else {
                p += 2 * abs_dx;
            }
            y += incy;
            vg_draw_point(x, y, color, size);
        }
    }
    
    return 0;
}

int (vg_draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color) {
    for (uint16_t i = 0; i < len; i++) {
        if (vg_draw_pixel(x + i, y, color, true)) return 1;
    }
    return 0;
}

int (vg_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color) {
    for (uint16_t i = 0; i < height; i++) {
        if (vg_draw_hline(x, y + i, width, color)) return 1;
    }
    return 0;
}

uint8_t clear_bits(uint8_t color, uint8_t starting_pos){
    if (starting_pos == 8) return color;

    for (int i = (int)starting_pos; i < 8; i++){
        color &= (~BIT(i));
    }

    return color;
}

int (vg_draw_xpm)(xpm_image_t* img, int16_t x, int16_t y, bool secondBuf) {
    uint16_t toIgnoreX = 0, toIgnoreY = 0;
    uint16_t x_ = (uint16_t) x, y_ = (uint16_t) y;
    if (x < 0) {
        toIgnoreX = abs(x) - 1;
        x_ = 0;
    }
    if (y < 0) {
        toIgnoreY = abs(y) - 1;
        y_ = 0;
    }

    if (x_ > h_res || y_ > v_res) return 1;
    

    uint16_t real_width = (x_ + img->width) > h_res ? h_res - x_ : img->width;
    uint16_t real_height = (y_ + img->height) > v_res ? v_res - y_ : img->height;
    uint32_t pixel_size = img->size / (img->height * img->width);
    
    for (uint16_t row = 0 + toIgnoreY; row < real_height; row++){
        for (uint16_t col = 0 + toIgnoreX; col < real_width; col++){
            uint8_t* current_color_address = img->bytes + (img->size / img->height)*row + pixel_size*col;
            uint32_t current_color = 0;
            memcpy(&current_color, current_color_address, pixel_size);
            
            if (current_color == xpm_transparency_color(img->type)) continue;
            
            char* fb_address = secondBuf ? (char*)second_fb : (char*)main_fb;
            memcpy(fb_address + ((y_ - toIgnoreY + row)*h_res + x_ - toIgnoreX + col)*pixel_size, current_color_address, pixel_size);
        
        }
    }

    return 0;
}
