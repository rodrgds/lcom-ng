#include <lcom/lcf.h>
#include <lcom/xpm.h>

static void *video_mem = NULL;
vbe_mode_info_t mode_info;
static struct minix_mem_range mr;

int set_graphics_mode(uint16_t mode) {
    struct reg86 r;
    memset(&r, 0, sizeof(r));
    
    r.ax = 0x4F02;
    r.bx = mode | BIT(14);  
    r.intno = 0x10;
    
    if (sys_int86(&r) != OK) {
        printf("Error setting graphics mode\n");
        return 1;
    }
    return 0;
}

int map_vram(uint16_t mode) {
    mr.mr_base = mode_info.PhysBasePtr;
    mr.mr_limit = mr.mr_base + (mode_info.XResolution * mode_info.YResolution * (mode_info.BitsPerPixel/8));
    
    if(sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr)) return 1;
    
    video_mem = vm_map_phys(SELF, (void*)mr.mr_base, mr.mr_limit - mr.mr_base);
    return video_mem == MAP_FAILED;
}

int draw_pixel(uint16_t x, uint16_t y, uint32_t color) {
    if(x >= mode_info.XResolution || y >= mode_info.YResolution) return 1;
    
    unsigned bytes_per_pixel = mode_info.BitsPerPixel / 8;
    uint8_t *ptr = (uint8_t*)video_mem + (y * mode_info.XResolution + x) * bytes_per_pixel;
    memcpy(ptr, &color, bytes_per_pixel);
    return 0;
}

int (vg_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color) {
    for(uint16_t i = x; i < x + width; i++) {
        for(uint16_t j = y; j < y + height; j++) {
            if(draw_pixel(i, j, color)) return 1;
        }
    }
    return 0;
}




int print_xpm(const char *xpm[], uint16_t x, uint16_t y) {
    xpm_image_t img;
    uint8_t *c = xpm_load((xpm_map_t)xpm, XPM_INDEXED, &img);
    if (c == NULL) return 1;

    for (int height = 0; height < img.height; height++) {
        for (int width = 0; width < img.width; width++) {
            if (draw_pixel(x + width, y + height, *c) != 0) {
                free(c);
                return 1;
            }
            c++;
        }
    }
    
    free(c - (img.height * img.width));
    return 0;
}
