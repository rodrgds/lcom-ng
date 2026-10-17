#include <lcom/lcf.h>

#include "graphics.h"
#include <math.h>

uint8_t *video_mem;
uint8_t *second_video_mem;

unsigned h_res;
unsigned v_res;
unsigned bytes_per_pixel;

unsigned int vram_base;
unsigned int vram_size;
vbe_mode_info_t mode_info;

struct minix_mem_range addr;

int r;		

void swap_buffers() {
    memcpy(video_mem, second_video_mem, vram_size);
}

int (_vg_init)(uint16_t mode) {
    memset(&mode_info, 0, sizeof(mode_info));
    if (vbe_get_mode_info(mode, &mode_info) != 0) return 1;

    h_res = mode_info.XResolution;
    v_res = mode_info.YResolution;

    bytes_per_pixel =  ((mode_info.BitsPerPixel + 7)/ 8.0);

    vram_base = mode_info.PhysBasePtr;
    vram_size = h_res * v_res * bytes_per_pixel;

    addr.mr_base = mode_info.PhysBasePtr;	
    addr.mr_limit = addr.mr_base + vram_size;  

    if(sys_privctl(SELF, SYS_PRIV_ADD_MEM, &addr)) panic("sys_privctl failed: %d\n", r);

    video_mem = vm_map_phys(SELF, (void *)addr.mr_base, vram_size);

    second_video_mem = (uint8_t *)malloc(vram_size);
    memset(second_video_mem, 0xFFFFFF, vram_size);

    if(video_mem == NULL) panic("couldn't map video memory");
    
    if (enter_video_mode(mode) != 0) return 1;

    return 0;
}

int (enter_video_mode)(uint16_t mode) {
    reg86_t reg;

    memset(&reg, 0, sizeof(reg));
    reg.intno = 0x10;
    reg.ah = 0x4F;
    reg.al = 0x02;
    reg.bx = BIT(14) | mode;

    if(sys_int86(&reg) != 0 ) return 1;

    return 0;
}

int (vg_draw_pixel)(uint16_t x, uint16_t y, uint32_t color) {
    if(x > h_res || y > v_res || x < 0 || y < 0) return 1;

    unsigned int pos = (y * h_res + x) * bytes_per_pixel;

    memcpy(&second_video_mem[pos], &color, bytes_per_pixel);

    return 0;
}

int (bitsPerPixel)() {
    return mode_info.BitsPerPixel;
}

void printXPM(uint16_t x, uint16_t y,  uint8_t* pixmap, xpm_image_t *img) {
    for(int i = 0; i < img->height; i++) {
      for(int j = 0; j < img->width; j++) {
        vg_draw_pixel(x + j, y + i,*(pixmap + (j + i * img->width) * bytes_per_pixel));
      }
  }
}

int (vg_draw_xpm)(xpm_map_t xpm, uint16_t x, uint16_t y) {
    xpm_image_t img;
    if(xpm_load(xpm, XPM_8_8_8_8, &img) == 0x0) return 1;

    uint32_t* map = (uint32_t*) img.bytes;

    for (unsigned int i = x ; i < img.width ; i++) {
        for (unsigned int j = y ; j < img.height; j++) {
            vg_draw_pixel(i,j,*(map + i + j * h_res));
        }
    }

    return 0;
}
