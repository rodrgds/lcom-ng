#include <lcom/lcf.h>

#include "GraphicCard.h"


int (change_graphics_mode)(uint16_t mode)
{
    reg86_t r86;

    memset(&r86, 0, sizeof(r86));

    r86.intno = 0x10; /* BIOS video services */
    r86.ah = 0x4F;    /* Set Video Mode function */
    r86.al = 0x02;    /* 80x25 text mode */
    r86.bx = BIT(14) | mode;

    return(sys_int86(&r86) != OK);
}

int (map_vram)(uint16_t mode)
{
    
    memset(&mode_inf, 0, sizeof(mode_inf));
    if(vbe_get_mode_info(mode, &mode_inf))
    {
        printf("map_memory vbe_get_mode_info error");
        return 1;
    }

    struct minix_mem_range phys_addr;
    phys_addr.mr_base = mode_inf.PhysBasePtr;
    phys_addr.mr_limit = phys_addr.mr_base + mode_inf.XResolution * mode_inf.YResolution * ((mode_inf.BitsPerPixel + 7) / 8);

    if(sys_privctl(SELF, SYS_PRIV_ADD_MEM, &phys_addr))
    {
        printf("map_memory sys_privctl error");
        return 1;
    }

    front_buffer = vm_map_phys(SELF, (void*) phys_addr.mr_base, mode_inf.XResolution * mode_inf.YResolution * ((mode_inf.BitsPerPixel + 7) / 8));
    back_buffer = (uint8_t *)malloc(mode_inf.XResolution * mode_inf.YResolution * ((mode_inf.BitsPerPixel + 7) / 8));

    if(front_buffer == NULL)
    {
        return 1;
    }
    if(back_buffer == NULL)
    {
        return 1;
    }

    return 0;
}

int (get_color)(uint32_t color1, uint32_t* color2)
{
    if(mode_inf.BitsPerPixel == 32)
    {
        *color2 = color1;
    }
    else
    {
        *color2 = color1 & (BIT(mode_inf.BitsPerPixel) - 1);
    }
    return 0;
}

int (vg_draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color)
{
    for(unsigned i = 0; i < len; i++)
    {
        if(vg_draw_pixel(x + i, y, color) == 1)
        {
            return 1;
        }
    }
    return 0;
}

int (vg_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color)
{
    for(unsigned i = 0; i < height; i++)
    {
        if(vg_draw_hline(x, y+i, width, color) == 1)
        {
            return 1;
        }
    }
    return 0;
}

int (vg_draw_pixel)(uint16_t x, uint16_t y, uint32_t color)
{
    if(x >= mode_inf.XResolution || y >= mode_inf.YResolution)
    {
        printf("\nout of bounds pixel");
        return 1;
    }

    unsigned bytes_per_pixel = (mode_inf.BitsPerPixel + 7) / 8;

    if(memcpy(&back_buffer[(mode_inf.XResolution * y + x) * bytes_per_pixel], &color, bytes_per_pixel) == NULL)
    {
        return 1;
    }
    return 0;
}

uint32_t (get_rgb)(uint32_t first, int i, int j, uint8_t step)
{
    uint32_t r = ((1 << mode_inf.RedMaskSize) - 1) & (first >> mode_inf.RedFieldPosition);
    r = (r + i * step) % (1 << mode_inf.RedMaskSize);
    uint32_t g = ((1 << mode_inf.GreenMaskSize) - 1) & (first >> mode_inf.GreenFieldPosition);
    g = (g + j * step) % (1 << mode_inf.GreenMaskSize);
    uint32_t b = ((1 << mode_inf.BlueMaskSize) - 1) & (first >> mode_inf.BlueFieldPosition);
    b = (b + (i + j) * step) % (1 << mode_inf.BlueMaskSize);
    return (r << mode_inf.RedFieldPosition) | (g << mode_inf.GreenFieldPosition) | (b << mode_inf.BlueFieldPosition);
}

uint32_t (normalize_color)(uint32_t color)
{
    uint32_t r = (color & 0xff0000) >> 16;
    uint32_t g = (color & 0x00ff00) >> 8;
    uint32_t b = (color & 0x0000ff);
    return ((r & 0xf8) << 8) | ((g & 0xfc) << 3) | ((b & 0xf8) >> 3);
}

int (draw_xpm)(uint16_t *sprite, xpm_image_t img, uint16_t x, uint16_t y) 
{
    for (int h = 0 ; h < img.height ; h++) {
        for (int w = 0 ; w < img.width ; w++) {
            if(*sprite != 0x000000)
            {
                uint16_t pixel_x = x + w;
                uint16_t pixel_y = y + h;
                if (pixel_x < mode_inf.XResolution && pixel_y < mode_inf.YResolution) {
                    if (vg_draw_pixel(pixel_x, pixel_y, *sprite)) return 1;
                }
            }
            sprite++;
        }
    }
    return 0;
}

int (video_switch)()
{
    if(memcpy(front_buffer, back_buffer, mode_inf.XResolution * mode_inf.YResolution * ((mode_inf.BitsPerPixel + 7) / 8)) == NULL)
    {
        return 1;
    }
    return 0;
}

void (clear_buffer)()
{
    memset(back_buffer, 0, mode_inf.XResolution * mode_inf.YResolution * ((mode_inf.BitsPerPixel + 7) / 8));
}
