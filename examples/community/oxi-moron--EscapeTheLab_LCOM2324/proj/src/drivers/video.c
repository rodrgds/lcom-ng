#include "video.h"
#include "VBE.h"

static char *video_mem, *second_video_mem;
static vbe_mode_info_t vmi;

int (vg_start) (uint16_t mode) {
    if (vg_set_mode(mode) != 0) {
        printf("ERROR: %s\n", __func__);
        return 1;
    }

    if (vg_map_vram(mode) != 0) {
        printf("ERROR: %s\n", __func__);
        vg_exit();
        return 1;
    }

    return 0;
}

int (vg_get_resolution) (uint32_t* hres, uint32_t* vres) {
    *hres = vmi.XResolution;
    *vres = vmi.YResolution;

    return 0;
}
int (vg_set_mode) (uint16_t mode) {
    reg86_t reg86;
    memset(&reg86, 0, sizeof(reg86));

    reg86.intno = 0x10;
    reg86.ax = 0x4F02;
    reg86.bx = 1 << 14 | mode;

    if (sys_int86(&reg86) != OK) {
        printf("ERROR: %s\n", __func__);
        return 1;
    }

    return 0;
}

int (vg_map_vram) (uint16_t mode) {

    vbe_mode_info_t vbe_mode_info;

    if (vbe_get_mode_info(mode, &vbe_mode_info) != 0) {
        printf("ERROR: %s\n", __func__ );
        return 1;
    }

    vmi = vbe_mode_info;

    int r;
    struct minix_mem_range mr;

    unsigned int vram_base = vbe_mode_info.PhysBasePtr;
    unsigned int vram_size = vmi.XResolution * vmi.YResolution * (vmi.BitsPerPixel / 8);

    mr.mr_base = (phys_bytes) vram_base;
    mr.mr_limit = mr.mr_base + vram_size;

    if( OK != (r = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr)))
        panic("sys_privctl (ADD_MEM) failed: %d\n", r);

    video_mem = vm_map_phys(SELF, (void *)mr.mr_base, vram_size);
    second_video_mem = malloc(vram_size);

    if(video_mem == MAP_FAILED)
        panic("couldn’t map video memory");

    return 0;
}

void (swap_buffer) () {
    memcpy(video_mem, second_video_mem, (vmi.BytesPerScanLine / vmi.XResolution) * vmi.XResolution * vmi.YResolution);
}

int (vg_draw_pixel) (uint16_t x, uint16_t y, uint32_t color) {

    char* pixel = second_video_mem + (((vmi.XResolution * y) + x) * (vmi.BitsPerPixel / 8));
    if (memcpy(pixel, &color, vmi.BitsPerPixel / 8) == NULL) return 1;

    return 0;
}

int (vg_draw_vline) (uint16_t x, uint16_t y, uint16_t len, uint32_t color) {

    for (int i = 0; i < len; i++) {
        vg_draw_pixel(x, y + i, color);
    }

    return 0;
}

int (vg_draw_vline_colormap) (uint16_t x, uint16_t y, uint16_t len, uint32_t* color_map) {

    for (uint32_t i = 0; i < len; i++) {
        vg_draw_pixel(x, y + i, color_map[i % 16]);
    }

    return 0;
}

int (vg_draw_hline) (uint16_t x, uint16_t y, uint16_t len, uint32_t color) {

    for (int i = 0; i < len; i++) {
        vg_draw_pixel(x + i, y, color);
    }

    return 0;
}

int (vg_draw_rectangle) (uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color) {

    for (int i = 0; i < height; i++) {
        vg_draw_hline(x, y + i, width, color);
    }

    return 0;
}

int (vg_draw_xpm) (uint16_t x, uint16_t y, int16_t width, uint32_t size, uint8_t* pixmap) {
    for (uint32_t i = 0; i < size / 3; i++) {
        uint32_t color = (pixmap[i * 3 + 2] << 16) | (pixmap[i * 3 + 1] << 8) | pixmap[i * 3];
        if (x + i % width < vmi.XResolution && y + i / width < vmi.YResolution)
            vg_draw_pixel(x + i % width, y + i / width, color);
    }

    return 0;
}

void (vg_free_buffer) () {
    free(second_video_mem);
}
