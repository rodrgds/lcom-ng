#include <lcom/lcf.h>

#include <stdint.h>
#include <stdio.h>

#include "graphics.h"

static uint16_t hres; // X res
static uint16_t vres; // Y res
static uint8_t bits_per_pixel;
void* address;

void *(map_adress)(uint16_t mode) {
    vbe_mode_info_t info;
    vbe_get_mode_info(mode, &info);

    hres = info.XResolution;
    vres = info.YResolution;
    bits_per_pixel = info.BitsPerPixel;

    int r;
    struct minix_mem_range mr; /*physical memory range*/
    unsigned int vram_base = info.PhysBasePtr; /* VRAM's physical addresss */
    unsigned int vram_size = hres * vres * ((bits_per_pixel+7)/8); /*VRAM’s size, but you can usethe frame-buffer size, instead*/
    void* video_mem; /*frame-buffer VM address*/

    /*Allow memory mapping*/

    mr.mr_base = (phys_bytes) vram_base;
    mr.mr_limit = mr.mr_base + vram_size;

    if( OK != (r = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr)))
        panic("sys_privctl (ADD_MEM) failed: %d\n", r);

    /*Map memory*/

    video_mem = vm_map_phys(SELF, (void *)mr.mr_base, vram_size);
    if(video_mem == MAP_FAILED)
        panic("couldn't map video memory");

    return video_mem;
}

int (gpu_mode)(uint16_t mode) {
    struct reg86 r;
    struct minix_mem_range mr;
    mr.mr_base = 0;
    mr.mr_limit = 1 << 20;
    memset(&r, 0, sizeof(r));
    r.ax = 0x4F02; // VBE call, function 02 -- set VBE mode
    r.bx = 1<<14|mode; // set bit 14: linear framebuffer
    r.intno = 0x10;
    if( sys_int86(&r) != OK) {
        printf("set_vbe_mode: sys_int86() failed \n");
        return 1;
    }
    return 0;
}

uint16_t getHres() {return hres;}

uint16_t getVres() {return vres;}

void* (vg_init)(uint16_t mode) {
    address = map_adress(mode);
    if (address == NULL) return NULL;
    if (gpu_mode(mode)) return NULL;
    return address;
}


