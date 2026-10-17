#include "../include/graphic.h"

static void *video_mem;
static unsigned h_res;
static unsigned v_res;
static unsigned bits_per_pixel;
static unsigned bytes_per_pixel;

static uint8_t red_mask;
static uint8_t green_mask;
static uint8_t blue_mask;
static uint8_t red_position;
static uint8_t green_position;
static uint8_t blue_position;

static uint16_t video_mode;
static vbe_mode_info_t mode_info;

void *
(vg_init)(uint16_t mode) 
{

    video_mode = mode;

    struct minix_mem_range mr;
    int r;

    vbe_get_mode_info(mode, &mode_info);
    h_res = mode_info.XResolution;
    v_res = mode_info.YResolution;
    bits_per_pixel = mode_info.BitsPerPixel;
    bytes_per_pixel = (mode_info.BitsPerPixel + 7) / 8;
    unsigned vram_base = (phys_bytes)mode_info.PhysBasePtr;
    unsigned vram_size = h_res * v_res * bytes_per_pixel;

    red_mask = mode_info.RedMaskSize;
    green_mask = mode_info.GreenMaskSize;
    blue_mask = mode_info.BlueMaskSize;

    red_position = mode_info.RedFieldPosition;
    green_position = mode_info.GreenFieldPosition;
    blue_position = mode_info.BlueFieldPosition;

    mr.mr_base = vram_base;
    mr.mr_limit = vram_base + vram_size;
    
    if (OK != (r = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr)))
        panic("sys_privctl (ADD_MEM) failed: %d\n", r);
    
    video_mem = vm_map_phys(SELF, (void*)mr.mr_base, (h_res * v_res * bytes_per_pixel));
    if (video_mem == MAP_FAILED)
        panic("couldn’t map video memory");

    reg86_t reg86;
    memset(&reg86, 0, sizeof(reg86));
    reg86.ax = SET_VBE_MODE;
    reg86.bx = BIT(14) | mode;
    reg86.intno = BIOS_VIDEO_SERVICES;
    if (sys_int86(&reg86) != OK) {
        printf("set_vbe_mode: sys_int86() failed \n");
        return NULL;
    }
    return video_mem;
}

int
(vg_draw_pixel)(uint16_t x, uint16_t y, uint32_t color)
{
    if (x < h_res && y < v_res) {
      char* init_address = (char*)video_mem + (y * h_res + x) * bytes_per_pixel;

        if (video_mode != MODE_INDEXED) {
            uint32_t red, green, blue;
            red = (color >> 16) & (BIT(red_mask) - 1);
            green = (color >> 8) & (BIT(green_mask) - 1);
            blue = (color) & (BIT(blue_mask) - 1);
            color = (red << red_position) | (green << green_position) | (blue << blue_position);
        }

      uint32_t prev_color;
      memcpy(&prev_color, init_address, bytes_per_pixel);

      if (prev_color != color)
        memcpy(init_address, &color, bytes_per_pixel);
      
      return 0;
    }
    return 1;
}


int
(vg_draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color) 
{
    for (unsigned int i = x; i < (x + len); i++)
        if (vg_draw_pixel(i, y, color)) return 1;
    return 0;
}

int
(vg_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color) 
{
    for (unsigned int i = y; i < y + height; i++)
        if (vg_draw_hline(x, i, width, color)) return 1;
    return 0;
}

int 
paint_screen(uint32_t color) 
{
    for (unsigned i = 0; i < h_res; i++) {
        for (unsigned j = 0; j < v_res; j++) {
            if (vg_draw_pixel(i, j, color)) return 1;
        }
    }
    return 0;
}

int
(vg_xpm_img_draw)(xpm_image_t* image, uint16_t x, uint16_t y)
{
    for (uint16_t i = 0; i < image->width; i++) {
        for (uint16_t j = 0; j < image->height; j++) {
            uint32_t pixelcolor = 0; 
            memcpy(&pixelcolor, image->bytes+(i + j * image->width)*bytes_per_pixel, bytes_per_pixel);
            if (vg_draw_pixel(x + i, y + j, pixelcolor)) return 1;
        }
    }
    return 0;

}

unsigned
get_hres()
{
    return h_res;
}

unsigned
get_vres()
{
    return v_res;
}

