//
// Created by skidr on 14/11/2019.
//

#include <lcom/lcf.h>
#include <machine/int86.h>
#include <lcom/video_gr.h>
#include <lcom/lab5.h>
#include <lcom/liblm.h>
#include <lcom/vbe.h>
#include <lcom/pixmap.h>

#include "video_gr.h"
#include "timer.h"
#include "tetris.h"

static int x_res;
static int y_res;
static unsigned bits_per_pixel;
static unsigned int vram_base;
static unsigned int vram_size;
static uint8_t memory_model;
static uint8_t red_mask_size;
static uint8_t green_mask_size;
static uint8_t blue_mask_size;
static char *video_mem;
static char *aux_buffer;



void *(vg_init)(uint16_t mode) {
    struct reg86 r86;

    memset(&r86, 0, sizeof(r86));

    r86.intno = 0x10;
    r86.ax = 0x4f02;
    r86.bx = 1 << 14 | mode;

    vbe_mode_info_t vbe;

    vbe_get_mode_info(mode, &vbe);

    x_res = vbe.XResolution;
    y_res = vbe.YResolution;
    bits_per_pixel = vbe.BitsPerPixel;
    memory_model = vbe.MemoryModel;
    red_mask_size = vbe.RedMaskSize;
    green_mask_size = vbe.GreenMaskSize;
    blue_mask_size = vbe.BlueMaskSize;

    struct minix_mem_range mr;
    vram_base = (phys_bytes) vbe.PhysBasePtr;
    vram_size = x_res * y_res * (bits_per_pixel / 8);

    mr.mr_base = vram_base;
    mr.mr_limit = mr.mr_base + vram_size;

    int r;

    if (OK != (r = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr)))
        panic("sys_privctl (ADD_MEM) failed: %d\n", r);

    /* Map memory */

    video_mem = vm_map_phys(SELF, (void *)mr.mr_base, vram_size);

    if (video_mem == MAP_FAILED)
        panic("couldn't map video memory");

    if (sys_int86(&r86) != OK)
    {
        printf("set_vbe_mode: sys_int86() failed \n");
        return NULL;
    }

    aux_buffer = (char *) malloc(vram_size);
    return video_mem;
}

void change_color(uint16_t x, uint16_t y, uint32_t color) {
    if (x < x_res && x >= 0 && y < y_res && y >= 0) {
        unsigned long pixelposition = (x + x_res * y) * (bits_per_pixel / 8);

        if (bits_per_pixel == 8) {
            video_mem[pixelposition] = (unsigned char) color;
        } else {
            unsigned long *base_video_mem = (unsigned long *) video_mem;
            base_video_mem[pixelposition] = color;
        }
    }
}

int vg_vbe_controller_info(struct vg_vbe_contr_info *vbeContrInfo) {
    mmap_t temp;
    if (lm_alloc(sizeof(struct vg_vbe_contr_info), &temp) == NULL)
        return 1;

    struct reg86 r86;
    memset(&r86, 0, sizeof(r86));
    r86.intno = 0x10;
    r86.ax = 0x4F00;
    r86.es = PB2BASE(temp.phys);
    r86.di = PB2OFF(temp.phys);

    if (sys_int86(&r86) != 0) {
        printf("error::sys_int86(&r86)\n");
        return 1;
    }

    memcpy(vbeContrInfo, temp.virt, temp.size);
    lm_free(&temp);
    return 0;
}

int getHorResolution() { return x_res; }

int getVerResolution() { return y_res; }

char* getGraphicsBuffer() {return video_mem; }

char* getAuxBuffer() { return aux_buffer; }

unsigned int getVRAMSize() { return vram_size; }

int doubleBuffering() {
    memcpy(video_mem, aux_buffer, vram_size);
    // free(aux_buffer);
    // aux_buffer = (char *) malloc(vram_size);
    return 0;
}


int resetSecondBuffer() {
    memset(aux_buffer, 0, vram_size);
    return 0;
}

