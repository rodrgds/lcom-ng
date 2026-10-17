#include <lcom/lcf.h>
#include <stdint.h>
#include <math.h>
#include "video.h"
#include "i8042.h"


static uint16_t h_res;
static uint16_t v_res;
static uint8_t bits_per_pixel;
static uint8_t bytes_per_pixel;
static uint8_t red_mask, blue_mask, green_mask;
static uint8_t red_pos, green_pos, blue_pos;
static char *video_mem; 
static char *double_buff;
static vbe_mode_info_t vbe;

void refresh(){
    memcpy(video_mem,double_buff,h_res*v_res * bytes_per_pixel);
}

void clear_sec_buff(){
    memset(double_buff,0,sizeof(uint8_t)*h_res*v_res*bytes_per_pixel);
}

void *(vg_init)(uint16_t mode) {
    uint32_t numPixel;


    vbe_get_mode_info_mine(mode, &vbe);

    h_res = vbe.XResolution;
    v_res = vbe.YResolution;
    bits_per_pixel = vbe.BitsPerPixel;
    bytes_per_pixel = ceil((float)bits_per_pixel / 8.0);
    red_mask = vbe.RedMaskSize;
    green_mask = vbe.GreenMaskSize;
    blue_mask = vbe.BlueMaskSize;
    red_pos = vbe.RedFieldPosition;
    green_pos = vbe.GreenFieldPosition;
    blue_pos = vbe.BlueFieldPosition;

    numPixel = h_res * v_res;
    int r1;
    struct minix_mem_range mr;  /* physical memory range */
    unsigned int vram_base = vbe.PhysBasePtr; /* VRAM’s physical addresss */
    unsigned int vram_size = numPixel * bytes_per_pixel; /* VRAM’s size, but you can use the frame-buffer size, instead */
    
    /* Allow memory mapping */
    mr.mr_base = (phys_bytes) vram_base;
    mr.mr_limit = mr.mr_base + vram_size;
    if( OK != (r1 = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr))) {
        panic("sys_privctl (ADD_MEM) failed: %d\n", r1);
    }
    /* Map memory */
    video_mem = vm_map_phys(SELF, (void *)mr.mr_base, vram_size);
    if(video_mem == MAP_FAILED) {
        panic("couldn’t map video memory");
    }

    double_buff = (char * )malloc(vram_size);

    
    reg86_t r;
    memset(&r, 0, sizeof(r));
    r.ax = 0x4F02; // VBE call, function 02 -- set VBE mode
    r.bx = 1<<14|mode; // set bit 14: linear framebuffer
    r.intno = 0x10;
    if (sys_int86(&r) != OK ) {
        printf("set_vbe_mode: sys_int86() failed \n");
        return NULL;
    }
    return video_mem;
}

void vg_color_pixel(uint16_t x, uint16_t y, uint32_t color) {
    if ((x > 0 && x < h_res) &&(y>0 && y< v_res)){
        char *aux_mem = double_buff + (y * h_res * bytes_per_pixel) + x * bytes_per_pixel;
        for (int j = 0; j < bytes_per_pixel; j++) {
            *aux_mem = (color >> (8 * j));
            aux_mem++;
        }
    }
}

int (vbe_get_mode_info_mine)(uint16_t mode, vbe_mode_info_t* vmi_p) {
    mmap_t map;
    reg86_t r;
    phys_bytes buf;

    memset(&r, 0, sizeof(r));
    if (lm_alloc(sizeof(vbe_mode_info_t), &map) == NULL) {
        return 1;
    }

    buf = map.phys;

    r.ax = 0x4F01;
    r.es = PB2BASE(buf);
    r.di = PB2OFF(buf);
    r.cx = mode;
    r.intno = 0x10;

    if (sys_int86(&r) != OK ) {
        printf("get_vbe_mode_info: sys_int86() failed \n");
        return 1;
    }
    *vmi_p = *(vbe_mode_info_t*) map.virt;
    lm_free(&map);
    return 0;
}

