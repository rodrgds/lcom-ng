#include "video.h"
#include "math.h"

int (vg_start)(uint16_t mode) {
    memset(&mode_info, 0, sizeof(mode_info));
    if (vbe_get_mode_info(mode, &mode_info) != 0) return 1;

    hres = mode_info.XResolution;
    vres = mode_info.YResolution;

    bytes_per_pixel = (mode_info.BitsPerPixel + 7) / 8;
    size_t frame_size = hres * vres * bytes_per_pixel;

    struct minix_mem_range mr;
    unsigned int vram_base = mode_info.PhysBasePtr; 
    unsigned int vram_size = frame_size*2;  
                        
    mr.mr_base = (phys_bytes) vram_base;
    mr.mr_limit = mr.mr_base + vram_size;

    if (sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr) != 0) return 1;

    video_mem = vm_map_phys(SELF, (void *) mr.mr_base, vram_size);

    if (video_mem == MAP_FAILED) return 1;

    second_buffer = malloc(frame_size);
    if (second_buffer == NULL) return 1;
    
    reg86_t reg86;
    memset(&reg86, 0, sizeof(reg86_t));

    reg86.intno = 0x10; 
    reg86.ah = 0x4F;
    reg86.al = 0x02;

    reg86.bx = BIT(14) | mode;

    if (sys_int86(&reg86) != 0) return 1;

    return 0;
}

int (vg_copy_buffer)() {
    if (memcpy(video_mem, second_buffer, hres * vres * bytes_per_pixel) == NULL) return 1;
    memset(second_buffer, 0, hres * vres * bytes_per_pixel);
    return 0;
}

uint16_t (get_hres)() {
    return hres;
}

int16_t (get_vres)() {
    return vres;
}

uint8_t (get_bytes_per_pixel)() {
    return bytes_per_pixel;
}

void (normalize_color)(uint32_t *color) {
    if (mode_info.BitsPerPixel != 32)
        *color &= BIT(mode_info.BitsPerPixel) - 1;
}

int (vg_draw_pixel)(uint16_t x, uint16_t y, uint32_t color) {
    if (x < 0 || y < 0) return 1;
    if (x >= hres || y >= vres) return 1;

    size_t idx = (hres * y + x) * bytes_per_pixel;
    if (memcpy(&second_buffer[idx], &color, bytes_per_pixel) == NULL) return 1;
    return 0;
}

int (vg_display_xpm)(xpm_map_t xpm, uint16_t x, uint16_t y) {
    xpm_image_t img;

    uint8_t *colors = xpm_load(xpm, XPM_INDEXED, &img); 

    for (int i = 0; i < img.height; i++) {
        for (int j = 0; j < img.width; j++) {
            if (vg_draw_pixel(x + j, y + i, *colors) != 0) return 1;
            colors++;
        }
    }

    return 0;
}

int (vg_erase_xpm)(xpm_map_t xpm, uint16_t x, uint16_t y) {
    xpm_image_t img;

    uint8_t *colors = xpm_load(xpm, XPM_INDEXED, &img); 

    for (int i = 0; i < img.height; i++) {
        for (int j = 0; j < img.width; j++) {
            if (vg_draw_pixel(x + j, y + i, 0) != 0) return 1;
            colors++;
        }
    }

    return 0;
}
