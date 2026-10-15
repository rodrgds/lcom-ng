#include <lcom/lcf.h>
#include <math.h>

#include "macros.h"
#include "graphics.h"
#include <stdint.h>
#include <stdio.h>

uint16_t xRes;
uint16_t yRes;
uint8_t bitsPerPixel;
uint8_t bytesPerPixel;
uint8_t *video_mem;
uint8_t *buffer;
uint8_t redMask;
uint8_t greenMask;
uint8_t blueMask;
vbe_mode_info_t vmi;

int vg_set_mode(uint16_t mode)
{

    reg86_t reg;
    memset(&reg, 0, sizeof(reg));
    reg.ah = VBE_CALL;
    reg.al = VBE_SET_MODE;
    reg.bx = VBE_LINEAR_FRAME | mode;
    reg.intno = VBE_INT;
    if (sys_int86(&reg) != OK)
    {
        printf("set_vbe_mode: sys_int86() failed \n", mode);
        return 1;
    }
    return 0;
}

void *(vg_init)(uint16_t mode)
{

    int r;
    struct minix_mem_range mr;
    unsigned int vram_base;
    unsigned int vram_size;

    vbeGetModeInfo(mode, &vmi);

    bitsPerPixel = vmi.BitsPerPixel;
    bytesPerPixel = bitsPerPixel / 8;
    if ((bitsPerPixel % 8 != 0))
    {
        bytesPerPixel++;
    }
    xRes = vmi.XResolution;
    yRes = vmi.YResolution;
    redMask = vmi.RedMaskSize;
    greenMask = vmi.GreenMaskSize;
    blueMask = vmi.BlueMaskSize;
    vram_base = vmi.PhysBasePtr;
    vram_size = xRes * yRes * bytesPerPixel;
    mr.mr_base = (phys_bytes)vmi.PhysBasePtr;
    mr.mr_limit = mr.mr_base + vram_size;

    if (OK != (r = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr)))
        panic("sys_privctl (ADD_MEM) failed: %d\n", r);
    video_mem = vm_map_phys(SELF, (void *)mr.mr_base, vram_size);

    buffer = (uint8_t *)malloc(vram_size);

    if (video_mem == MAP_FAILED)
        panic("couldn’t map video memory");

    vg_set_mode(mode);

    return video_mem;
}

void flipBuffer()
{
    memcpy(video_mem, buffer, xRes * yRes * bytesPerPixel);
}

int vbeGetModeInfo(uint16_t mode, vbe_mode_info_t *vmi_p)
{
    phys_bytes physAddr;
    reg86_t reg;
    mmap_t m;
    memset(&reg, 0, sizeof(reg));
    lm_alloc(sizeof(vbe_mode_info_t), &m);
    physAddr = m.phys;
    reg.ah = VBE_CALL;
    reg.al = VBE_MODE_INFO;
    reg.es = PB2BASE(physAddr);
    reg.di = PB2OFF(physAddr);
    reg.cx = mode;
    reg.intno = VBE_INT;
    if (sys_int86(&reg) != OK)
    {
        return 1;
    }
    *vmi_p = *(vbe_mode_info_t *)(m.virt);
    lm_free(&m);
    return 0;
}

void fillPixel(int x, int y, uint32_t color)
{

    if ((x < 0) || (x >= xRes) || (y < 0) || (y >= yRes))
        return;

    uint8_t *memPTR = buffer;
    uint8_t shift = SIZEOF_UINT32 - bitsPerPixel;
    memPTR += (y * xRes + x) * bytesPerPixel;
    color <<= shift;
    color >>= shift;
    for (uint8_t i = 0; i < bytesPerPixel; i++)
    {
        *memPTR = color >> (SIZEOF_BYTE * i);
        memPTR++;
    }
}

int draw_rectangle(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color)
{
    width += x - 1;
    height += y - 1;
    for (; y <= height; y++)
    {
        for (; x <= width; x++)
        {
            fillPixel(x, y, color);
        }
        x -= width;
    }
    return 0;
}

void drawLine(uint16_t xi, uint16_t yi, uint16_t xf, uint16_t yf, uint32_t color)
{
    float slope, dx, dy;
    uint16_t x, y, tmp;
    if (yi > yf || xi > xf)
    {
        tmp = xi;
        xi = xf;
        xf = tmp;
        tmp = yi;
        yi = yf;
        yf = tmp;
    }
    dx = xf - xi;
    dy = yf - yi;
    if (abs(dx) > abs(dy))
    {
        if (dy)
        {
            slope = dx / dy;
            for (x = xi; x <= xf; x++)
            {
                y = (x - xi) / slope + yi;
                fillPixel(x, y, color);
                fillPixel(x, y+1, color);
                fillPixel(x, y-1, color);
            }
        }
        else
        {
            for (x = xi; x <= xf; x++)
            {
                fillPixel(x, yi, color);
                fillPixel(x, yi+1, color);
                fillPixel(x, yi-1, color);
            }
        }
    }
    else
    {
        if (dx)
        {
            slope = dy / dx;
            for (y = yi; y <= yf; y++)
            {
                x = (y - yi) / slope + xi;
                fillPixel(x, y, color);
                fillPixel(x+1, y, color);
                fillPixel(x-1, y, color);
            }
        }
        else
        {
            for (y = yi; y <= yf; y++)
            {
                fillPixel(xi, y, color);
                fillPixel(xi+1, y, color);
                fillPixel(xi-1, y, color);
            }
        }
    }
}

void clearScreen()
{
    draw_rectangle(0, 0, 800, 600, 0x0);
}
